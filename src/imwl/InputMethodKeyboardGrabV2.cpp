// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodKeyboardGrabV2.h"

#include "X11KeyboardGrabber.h"
#include "common.h"
#include "shm_open_anon.h"

#include <QDateTime>
#include <QDebug>

#include <sys/mman.h>
#include <unistd.h>

InputMethodKeyboardGrabV2::InputMethodKeyboardGrabV2(wl::server::Seat *seat)
    : seat_(seat)
    , grabber_(std::make_unique<X11KeyboardGrabber>())
    , xkbContext_(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
    , state_({})
{
    xkb_rule_names rules = {
        .rules = getenv("XKB_DEFAULT_RULES"),
        .model = getenv("XKB_DEFAULT_MODEL"),
        .layout = getenv("XKB_DEFAULT_LAYOUT"),
        .variant = getenv("XKB_DEFAULT_VARIANT"),
        .options = getenv("XKB_DEFAULT_OPTIONS"),
    };
    xkbKeymap_.reset(
        xkb_keymap_new_from_names(xkbContext_.get(), &rules, XKB_KEYMAP_COMPILE_NO_FLAGS));
    xkbState_.reset(xkb_state_new(xkbKeymap_.get()));

    QObject::connect(grabber_.get(),
                     &X11KeyboardGrabber::keyEvent,
                     [this](int keycode, bool isRelease) {
                         sendKey(keycode, isRelease);
                         updateState(keycode, isRelease);
                     });
}

InputMethodKeyboardGrabV2::~InputMethodKeyboardGrabV2() { }

void InputMethodKeyboardGrabV2::sendKey(uint32_t keycode, bool isRelease)
{
    uint32_t ts = QDateTime::currentSecsSinceEpoch();
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        send_key(resource->handle,
                 nextSerial(),
                 ts,
                 keycode,
                 isRelease ? WL_KEYBOARD_KEY_STATE_RELEASED : WL_KEYBOARD_KEY_STATE_PRESSED);
    }
}

void InputMethodKeyboardGrabV2::resource_bind(wl::server::Resource *resource)
{
    auto res = genKeymapData(xkbKeymap_.get());

    send_keymap(resource->handle, XKB_KEYMAP_FORMAT_TEXT_V1, res.first, res.second);

    close(res.first);
}

void InputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_release(
    wl::server::Resource *resource)
{
    resource->destroy();
}

std::pair<int, size_t> InputMethodKeyboardGrabV2::genKeymapData(xkb_keymap *keymap)
{
    std::unique_ptr<char[]> keymapStr(xkb_keymap_get_as_string(keymap, XKB_KEYMAP_FORMAT_TEXT_V1));
    if (!keymapStr) {
        return std::make_pair(0, 0);
    }
    auto size = strlen(keymapStr.get());
    int fd = shm_open_anon();
    if (ftruncate(fd, size) != 0) {
        qWarning() << "ftruncate failed";
    }

    void *tmp = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (tmp == MAP_FAILED) {
        qWarning() << "mmap failed";
        close(fd);
        return std::make_pair(0, 0);
    }
    memcpy(tmp, keymapStr.get(), size);
    munmap(tmp, size);

    return std::make_pair(fd, size);
}

bool InputMethodKeyboardGrabV2::updateState(uint32_t keycode, bool isRelease)
{
    xkb_state_update_key(xkbState_.get(), keycode, isRelease ? XKB_KEY_UP : XKB_KEY_DOWN);

    State state = {};
    state.modsDepressed = xkb_state_serialize_mods(xkbState_.get(), XKB_STATE_MODS_DEPRESSED);
    state.modsLatched = xkb_state_serialize_mods(xkbState_.get(), XKB_STATE_MODS_LATCHED);
    state.modsLocked = xkb_state_serialize_mods(xkbState_.get(), XKB_STATE_MODS_LOCKED);
    state.group = xkb_state_serialize_layout(xkbState_.get(), XKB_STATE_LAYOUT_EFFECTIVE);

    if (memcmp(&state_, &state, sizeof(state)) == 0) {
        return false;
    }

    state_ = state;

    auto &resources = resourceMap();
    for (auto &[client, resource] : resources) {
        send_modifiers(resource->handle,
                       nextSerial(),
                       state.modsDepressed,
                       state.modsLatched,
                       state.modsLocked,
                       state.group);
    }

    return true;
}

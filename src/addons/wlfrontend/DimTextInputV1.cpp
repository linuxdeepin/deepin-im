// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DimTextInputV1.h"

#include "common/common.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"
#include "wl/client/ZwpInputMethodKeyboardGrabV2.h"

#include <linux/input.h>

#include <QDebug>

#include <sys/mman.h>

using namespace org::deepin::dim;

const zwp_input_method_v2_listener DimTextInputV1::imListener_ = {
    CallbackWrapper<&DimTextInputV1::activate>::func,
    CallbackWrapper<&DimTextInputV1::deactivate>::func,
    CallbackWrapper<&DimTextInputV1::surroundingText>::func,
    CallbackWrapper<&DimTextInputV1::textChangeCause>::func,
    CallbackWrapper<&DimTextInputV1::contentType>::func,
    CallbackWrapper<&DimTextInputV1::done>::func,
    CallbackWrapper<&DimTextInputV1::unavailable>::func,
};

const zwp_input_method_keyboard_grab_v2_listener DimTextInputV1::grabListener_ = {
    CallbackWrapper<&DimTextInputV1::keymap>::func,
    CallbackWrapper<&DimTextInputV1::key>::func,
    CallbackWrapper<&DimTextInputV1::modifiers>::func,
    CallbackWrapper<&DimTextInputV1::repeatInfo>::func,
};

static int32_t getTimestamp()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / (1000 * 1000);
}

DimTextInputV1::DimTextInputV1(
    Dim *dim,
    const std::shared_ptr<wl::client::ZwpInputMethodV2> &im,
    const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> &vk)
    : InputContext(dim)
    , im_(im)
    , vk_(vk)
    , state_(std::make_unique<State>())
    , xkbContext_(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
{
    zwp_input_method_v2_add_listener(im_->get(), &imListener_, this);
}

void DimTextInputV1::activate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qDebug() << "im activated:" << id();

    grab_ = im_->grabKeyboard();
    zwp_input_method_keyboard_grab_v2_add_listener(grab_->get(), &grabListener_, this);

    focusIn();
}

void DimTextInputV1::deactivate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qDebug() << "im deactivated:" << id();

    state_.reset(new State);
    grab_.reset();

    focusOut();
}

void DimTextInputV1::surroundingText(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] const char *text,
    [[maybe_unused]] uint32_t cursor,
    [[maybe_unused]] uint32_t anchor)
{
    qDebug() << "im surroundingText:" << id();
}

void DimTextInputV1::textChangeCause(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t cause)
{
    qDebug() << "im textChangeCause:" << id();
}

void DimTextInputV1::contentType(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t hint,
    [[maybe_unused]] uint32_t purpose)
{
    qDebug() << "im contentType:" << id();
}

void DimTextInputV1::done([[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
}

void DimTextInputV1::unavailable(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
}

void DimTextInputV1::keymap(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t format,
    int32_t fd,
    uint32_t size)
{
    qDebug() << "grab keymap:" << format << fd << size;

    if (format == WL_KEYBOARD_KEYMAP_FORMAT_NO_KEYMAP) {
        return;
    }

    void *ptr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        return;
    }

    xkbKeymap_.reset(xkb_keymap_new_from_string(xkbContext_.get(),
                                                static_cast<const char *>(ptr),
                                                XKB_KEYMAP_FORMAT_TEXT_V1,
                                                XKB_KEYMAP_COMPILE_NO_FLAGS));
    munmap(ptr, size);

    if (!xkbKeymap_) {
        return;
    }

    vk_->keymap(format, fd, size);

    xkbState_.reset(xkb_state_new(xkbKeymap_.get()));
    if (!xkbState_) {
        xkbKeymap_.reset();
        return;
    }

    modifierMask_[static_cast<uint8_t>(Modifiers::Shift)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Shift");
    modifierMask_[static_cast<uint8_t>(Modifiers::Lock)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Lock");
    modifierMask_[static_cast<uint8_t>(Modifiers::Control)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Control");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod1)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Mod1");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod2)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Mod2");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod3)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Mod3");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod4)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Mod4");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod5)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Mod5");
    modifierMask_[static_cast<uint8_t>(Modifiers::Alt)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Alt");
    modifierMask_[static_cast<uint8_t>(Modifiers::Meta)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Meta");
    modifierMask_[static_cast<uint8_t>(Modifiers::Super)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Super");
    modifierMask_[static_cast<uint8_t>(Modifiers::Hyper)] = 1
        << xkb_keymap_mod_get_index(xkbKeymap_.get(), "Hyper");
}

void DimTextInputV1::key(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t serial,
    uint32_t time,
    uint32_t key,
    uint32_t state)
{
    qDebug() << "grab key:" << serial << time << key << state;
    assert(xkbState_);

    xkb_keysym_t sym = xkb_state_key_get_one_sym(xkbState_.get(), key);
    InputContextKeyEvent ke(this,
                            static_cast<uint32_t>(sym),
                            key,
                            state_->modifiers,
                            state == WL_KEYBOARD_KEY_STATE_RELEASED,
                            time);
    bool res = keyEvent(ke);
    if (!res) {
        vk_->key(getTimestamp(), key, state);
        return;
    }

    auto list = getAndClearBatchList();
    for (auto &e : list) {
        if (std::holds_alternative<ForwardKey>(e)) {
            auto forwardKey = std::get<ForwardKey>(e);
            vk_->key(getTimestamp(),
                     forwardKey.keycode + XKB_HISTORICAL_OFFSET,
                     forwardKey.pressed ? WL_KEYBOARD_KEY_STATE_PRESSED
                                        : WL_KEYBOARD_KEY_STATE_RELEASED);
            continue;
        }

        if (std::holds_alternative<PreeditInfo>(e)) {
            auto preeditInfo = std::get<PreeditInfo>(e);
            im_->setPreeditString(preeditInfo.text.toStdString().c_str(),
                                  preeditInfo.cursorBegin,
                                  preeditInfo.cursorEnd);
            continue;
        }

        if (std::holds_alternative<CommitString>(e)) {
            auto text = std::get<CommitString>(e).text;
            im_->commitString(text.toStdString().c_str());
            continue;
        }
    }

    if (!list.empty()) {
        im_->commit(state_->serial++);
    }
}

void DimTextInputV1::modifiers(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t serial,
    uint32_t mods_depressed,
    uint32_t mods_latched,
    uint32_t mods_locked,
    uint32_t group)
{
    qDebug() << "grab modifiers:" << serial << mods_depressed << mods_latched << mods_locked
             << group;

    if (xkbState_) {
        xkb_state_component comp = xkb_state_update_mask(xkbState_.get(),
                                                         mods_depressed,
                                                         mods_latched,
                                                         mods_locked,
                                                         0,
                                                         0,
                                                         group);
        xkb_mod_mask_t mask = xkb_state_serialize_mods(xkbState_.get(), comp);
        state_->modifiers = 0;
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Shift)]) {
            state_->modifiers |= SHIFT_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Lock)]) {
            state_->modifiers |= LOCK_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Control)]) {
            state_->modifiers |= CONTROL_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod1)]) {
            state_->modifiers |= MOD1_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod2)]) {
            state_->modifiers |= MOD2_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod3)]) {
            state_->modifiers |= MOD3_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod4)]) {
            state_->modifiers |= MOD4_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod5)]) {
            state_->modifiers |= MOD5_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Alt)]) {
            state_->modifiers |= ALT_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Meta)]) {
            state_->modifiers |= META_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Super)]) {
            state_->modifiers |= SUPER_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Hyper)]) {
            state_->modifiers |= HYPER_MASK;
        }
    }

    if (vk_) {
        vk_->modifiers(mods_depressed, mods_latched, mods_locked, group);
    }
}

void DimTextInputV1::repeatInfo(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    int32_t rate,
    int32_t delay)
{
    qDebug() << "grab repeatInfo:" << rate << delay;
}

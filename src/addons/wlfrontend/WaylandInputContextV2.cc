#include "WaylandInputContextV2.h"

#include "utils.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"

#include <linux/input.h>

#include <QDebug>

#include <sys/mman.h>

using namespace org::deepin::dim;

const zwp_input_method_v2_listener WaylandInputContextV2::im_listener_ = {
    CallbackWrapper<&WaylandInputContextV2::activate>::func,
    CallbackWrapper<&WaylandInputContextV2::deactivate>::func,
    CallbackWrapper<&WaylandInputContextV2::surroundingText>::func,
    CallbackWrapper<&WaylandInputContextV2::textChangeCause>::func,
    CallbackWrapper<&WaylandInputContextV2::contentType>::func,
    CallbackWrapper<&WaylandInputContextV2::done>::func,
    CallbackWrapper<&WaylandInputContextV2::unavailable>::func,
};

const zwp_input_method_keyboard_grab_v2_listener WaylandInputContextV2::grab_listener_ = {
    CallbackWrapper<&WaylandInputContextV2::keymap>::func,
    CallbackWrapper<&WaylandInputContextV2::key>::func,
    CallbackWrapper<&WaylandInputContextV2::modifiers>::func,
    CallbackWrapper<&WaylandInputContextV2::repeatInfo>::func,
};

static int32_t getTimestamp()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / (1000 * 1000);
}

WaylandInputContextV2::WaylandInputContextV2(
    Dim *dim,
    const std::shared_ptr<WlType<zwp_input_method_v2>> &im,
    const std::shared_ptr<WlType<zwp_virtual_keyboard_v1>> &vk)
    : InputContext(dim)
    , im_(im)
    , vk_(vk)
    , state_(std::make_unique<State>())
    , xkb_context_(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
{
    zwp_input_method_v2_add_listener(im_->get(), &im_listener_, this);

    grab_ = std::make_shared<WlType<zwp_input_method_keyboard_grab_v2>>(
        zwp_input_method_v2_grab_keyboard(im_->get()));
    zwp_input_method_keyboard_grab_v2_add_listener(grab_->get(), &grab_listener_, this);
}

void WaylandInputContextV2::activate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im activated:" << id();
    focusIn();
}

void WaylandInputContextV2::deactivate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im deactivated:" << id();
    state_.reset(new State);
    focusOut();
}

void WaylandInputContextV2::surroundingText(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] const char *text,
    [[maybe_unused]] uint32_t cursor,
    [[maybe_unused]] uint32_t anchor)
{
    qWarning() << "im surroundingText:" << id();
}

void WaylandInputContextV2::textChangeCause(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t cause)
{
    qWarning() << "im textChangeCause:" << id();
}

void WaylandInputContextV2::contentType(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t hint,
    [[maybe_unused]] uint32_t purpose)
{
    qWarning() << "im contentType:" << id();
}

void WaylandInputContextV2::done([[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im done:" << id();
}

void WaylandInputContextV2::unavailable(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im unvailable:" << id();
}

void WaylandInputContextV2::keymap(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t format,
    int32_t fd,
    uint32_t size)
{
    qWarning() << "grab keymap:" << format << fd << size;

    if (format == WL_KEYBOARD_KEYMAP_FORMAT_NO_KEYMAP) {
        return;
    }

    void *ptr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        return;
    }

    xkb_keymap_.reset(xkb_keymap_new_from_string(xkb_context_.get(),
                                                 static_cast<const char *>(ptr),
                                                 XKB_KEYMAP_FORMAT_TEXT_V1,
                                                 XKB_KEYMAP_COMPILE_NO_FLAGS));
    munmap(ptr, size);

    if (!xkb_keymap_) {
        return;
    }

    zwp_virtual_keyboard_v1_keymap(vk_->get(), format, fd, size);

    xkb_state_.reset(xkb_state_new(xkb_keymap_.get()));
    if (!xkb_state_) {
        xkb_keymap_.reset();
        return;
    }

    modifierMask_[static_cast<uint8_t>(Modifiers::Shift)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Shift");
    modifierMask_[static_cast<uint8_t>(Modifiers::Lock)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Lock");
    modifierMask_[static_cast<uint8_t>(Modifiers::Control)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Control");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod1)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Mod1");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod2)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Mod2");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod3)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Mod3");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod4)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Mod4");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod5)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Mod5");
    modifierMask_[static_cast<uint8_t>(Modifiers::Alt)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Alt");
    modifierMask_[static_cast<uint8_t>(Modifiers::Meta)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Meta");
    modifierMask_[static_cast<uint8_t>(Modifiers::Super)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Super");
    modifierMask_[static_cast<uint8_t>(Modifiers::Hyper)] = 1
        << xkb_keymap_mod_get_index(xkb_keymap_.get(), "Hyper");
}

void WaylandInputContextV2::key(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t serial,
    uint32_t time,
    uint32_t key,
    uint32_t state)
{
    qWarning() << "grab key:" << serial << time << key << state;

    xkb_keysym_t sym = xkb_state_key_get_one_sym(xkb_state_.get(), key + 8);
    InputContextKeyEvent ke(this,
                            static_cast<uint32_t>(sym),
                            key,
                            state_->modifiers,
                            state == WL_KEYBOARD_KEY_STATE_RELEASED,
                            time);
    bool res = keyEvent(ke);
    if (!res) {
        zwp_virtual_keyboard_v1_key(vk_->get(), getTimestamp(), key, state);
        return;
    }

    auto list = getAndClearBatchList();
    for (auto &e : list) {
        if (std::holds_alternative<ForwardKey>(e)) {
            auto forwardKey = std::get<ForwardKey>(e);
            zwp_virtual_keyboard_v1_key(vk_->get(),
                                        getTimestamp(),
                                        forwardKey.keycode,
                                        forwardKey.pressed ? WL_KEYBOARD_KEY_STATE_PRESSED
                                                           : WL_KEYBOARD_KEY_STATE_RELEASED);
            continue;
        }

        if (std::holds_alternative<PreeditInfo>(e)) {
            auto preeditInfo = std::get<PreeditInfo>(e);
            zwp_input_method_v2_set_preedit_string(im_->get(),
                                                   preeditInfo.text.toStdString().c_str(),
                                                   preeditInfo.cursorBegin,
                                                   preeditInfo.cursorEnd);
            continue;
        }

        if (std::holds_alternative<CommitString>(e)) {
            auto text = std::get<CommitString>(e).text;
            zwp_input_method_v2_commit_string(im_->get(), text.toStdString().c_str());
            continue;
        }
    }

    if (!list.empty()) {
        zwp_input_method_v2_commit(im_->get(), state_->serial++);
    }
}

void WaylandInputContextV2::modifiers(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t serial,
    uint32_t mods_depressed,
    uint32_t mods_latched,
    uint32_t mods_locked,
    uint32_t group)
{
    qWarning() << "grab modifiers:" << serial << mods_depressed << mods_latched << mods_locked
               << group;

    if (xkb_state_) {
        xkb_state_component comp = xkb_state_update_mask(xkb_state_.get(),
                                                         mods_depressed,
                                                         mods_latched,
                                                         mods_locked,
                                                         0,
                                                         0,
                                                         group);
        xkb_mod_mask_t mask = xkb_state_serialize_mods(xkb_state_.get(), comp);
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
        zwp_virtual_keyboard_v1_modifiers(vk_->get(),
                                          mods_depressed,
                                          mods_latched,
                                          mods_locked,
                                          group);
    }
}

void WaylandInputContextV2::repeatInfo(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    int32_t rate,
    int32_t delay)
{
    qWarning() << "grab repeatInfo:" << rate << delay;
}

// void WaylandInputContextV2::updatePreedit(const QString &text,
//                                           int32_t cursorBegin,
//                                           int32_t cursorEnd)
// {
//     zwp_input_method_v2_set_preedit_string(im_->get(),
//                                            text.toStdString().c_str(),
//                                            cursorBegin,
//                                            cursorEnd);
// }

// void WaylandInputContextV2::updateCommitString(const QString &text)
// {
//     zwp_input_method_v2_commit_string(im_->get(), text.toStdString().c_str());
// }

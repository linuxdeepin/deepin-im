// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodKeyboardGrabV2.h"

#include "InputMethodV2.h"
#include "WlInputContext.h"
#include "common/common.h"
#include "wl/client/ZwpInputMethodKeyboardGrabV2.h"
#include "wl/client/ZwpVirtualKeyboardV1.h"

#include <linux/input.h>

#include <QDebug>

#include <sys/mman.h>

using namespace org::deepin::dim;

static int32_t getTimestamp()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / (1000 * 1000);
}

InputMethodKeyboardGrabV2::InputMethodKeyboardGrabV2(zwp_input_method_keyboard_grab_v2 *val, InputMethodV2* im)
    : wl::client::ZwpInputMethodKeyboardGrabV2(val)
    , im_(im)
{
}

void InputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_keymap(uint32_t format,
                                                                         int32_t fd,
                                                                         uint32_t size)
{
    if (format == WL_KEYBOARD_KEYMAP_FORMAT_NO_KEYMAP) {
        return;
    }

    void *ptr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        return;
    }

    im_->ic_->xkbKeymap_.reset(xkb_keymap_new_from_string(im_->ic_->xkbContext_.get(),
                                                static_cast<const char *>(ptr),
                                                XKB_KEYMAP_FORMAT_TEXT_V1,
                                                XKB_KEYMAP_COMPILE_NO_FLAGS));
    munmap(ptr, size);

    if (!im_->ic_->xkbKeymap_) {
        return;
    }

    im_->vk_->keymap(format, fd, size);

    im_->ic_->xkbState_.reset(xkb_state_new(im_->ic_->xkbKeymap_.get()));
    if (!im_->ic_->xkbState_) {
        im_->ic_->xkbKeymap_.reset();
        return;
    }

    modifierMask_[static_cast<uint8_t>(Modifiers::Shift)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Shift");
    modifierMask_[static_cast<uint8_t>(Modifiers::Lock)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Lock");
    modifierMask_[static_cast<uint8_t>(Modifiers::Control)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Control");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod1)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Mod1");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod2)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Mod2");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod3)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Mod3");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod4)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Mod4");
    modifierMask_[static_cast<uint8_t>(Modifiers::Mod5)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Mod5");
    modifierMask_[static_cast<uint8_t>(Modifiers::Alt)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Alt");
    modifierMask_[static_cast<uint8_t>(Modifiers::Meta)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Meta");
    modifierMask_[static_cast<uint8_t>(Modifiers::Super)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Super");
    modifierMask_[static_cast<uint8_t>(Modifiers::Hyper)] = 1
        << xkb_keymap_mod_get_index(im_->ic_->xkbKeymap_.get(), "Hyper");
}

void InputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_key(uint32_t serial,
                                                                      uint32_t time,
                                                                      uint32_t key,
                                                                      uint32_t state)
{
    assert(im_->ic_->xkbState_);

    xkb_keysym_t sym = xkb_state_key_get_one_sym(im_->ic_->xkbState_.get(), key);
    InputContextKeyEvent ke(im_->ic_.get(),
                            static_cast<uint32_t>(sym),
                            key,
                            im_->ic_->state_->modifiers,
                            state == WL_KEYBOARD_KEY_STATE_RELEASED,
                            time);
    bool res = im_->ic_->keyEvent(ke);
    if (!res) {
        im_->vk_->key(getTimestamp(), key, state);
        return;
    }

    if (!im_->ic_->isAsyncMode()) {
        im_->ic_->keyEventDispatch();
    }
}

void InputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_modifiers(uint32_t serial,
                                                                            uint32_t mods_depressed,
                                                                            uint32_t mods_latched,
                                                                            uint32_t mods_locked,
                                                                            uint32_t group)
{
    if (im_->ic_->xkbState_) {
        xkb_state_component comp = xkb_state_update_mask(im_->ic_->xkbState_.get(),
                                                         mods_depressed,
                                                         mods_latched,
                                                         mods_locked,
                                                         0,
                                                         0,
                                                         group);
        xkb_mod_mask_t mask = xkb_state_serialize_mods(im_->ic_->xkbState_.get(), comp);
        im_->ic_->state_->modifiers = 0;
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Shift)]) {
            im_->ic_->state_->modifiers |= SHIFT_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Lock)]) {
            im_->ic_->state_->modifiers |= LOCK_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Control)]) {
            im_->ic_->state_->modifiers |= CONTROL_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod1)]) {
            im_->ic_->state_->modifiers |= MOD1_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod2)]) {
            im_->ic_->state_->modifiers |= MOD2_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod3)]) {
            im_->ic_->state_->modifiers |= MOD3_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod4)]) {
            im_->ic_->state_->modifiers |= MOD4_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod5)]) {
            im_->ic_->state_->modifiers |= MOD5_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Alt)]) {
            im_->ic_->state_->modifiers |= ALT_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Meta)]) {
            im_->ic_->state_->modifiers |= META_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Super)]) {
            im_->ic_->state_->modifiers |= SUPER_MASK;
        }
        if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Hyper)]) {
            im_->ic_->state_->modifiers |= HYPER_MASK;
        }
    }

    if (im_->vk_) {
        im_->vk_->modifiers(mods_depressed, mods_latched, mods_locked, group);
    }
}

void InputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_repeat_info(int32_t rate,
                                                                              int32_t delay)
{
    // TODO
}

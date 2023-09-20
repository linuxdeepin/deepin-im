// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardV1.h"

#include "Compositor.h"
#include "DimTextInputV1.h"
#include "common.h"
#include "wl/server/Resource.h"

#include <xkbcommon/xkbcommon.h>

#include <sys/mman.h>

// static uint32_t toModifiers(KeyStates states)
// {
//     uint32_t modifiers = 0;
//     // We use Shift Control Mod1 Mod4
//     if (states.test(KeyState::Shift)) {
//         modifiers |= (1 << 0);
//     }
//     if (states.test(KeyState::Ctrl)) {
//         modifiers |= (1 << 1);
//     }
//     if (states.test(KeyState::Alt)) {
//         modifiers |= (1 << 2);
//     }
//     if (states.test(KeyState::Super)) {
//         modifiers |= (1 << 3);
//     }
//     return modifiers;
// }

VirtualKeyboardV1::VirtualKeyboardV1(QWaylandSeat *seat)
    : seat_(seat)
    , xkbContext_(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
{
}

VirtualKeyboardV1::~VirtualKeyboardV1() { }

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_keymap(wl::server::Resource *resource,
                                                       uint32_t format,
                                                       int32_t fd,
                                                       uint32_t size)
{
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

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_key(wl::server::Resource *resource,
                                                    uint32_t time,
                                                    uint32_t key,
                                                    uint32_t state)
{
    xkb_keysym_t sym = xkb_state_key_get_one_sym(xkbState_.get(), key);
    auto dti = dynamic_cast<Compositor *>(seat_->compositor())->getDimTextInputV1();
    dti->sendKeysym(nextSerial(), time, sym, state, state_.modifiers);
}

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_modifiers(wl::server::Resource *resource,
                                                          uint32_t mods_depressed,
                                                          uint32_t mods_latched,
                                                          uint32_t mods_locked,
                                                          uint32_t group)
{
    auto comp = xkb_state_update_mask(xkbState_.get(),
                                      mods_depressed,
                                      mods_latched,
                                      mods_locked,
                                      0,
                                      0,
                                      group);

    xkb_mod_mask_t mask = xkb_state_serialize_mods(xkbState_.get(), comp);
    state_.modifiers = 0;
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Shift)]) {
        state_.modifiers |= SHIFT_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Lock)]) {
        state_.modifiers |= LOCK_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Control)]) {
        state_.modifiers |= CONTROL_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod1)]) {
        state_.modifiers |= MOD1_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod2)]) {
        state_.modifiers |= MOD2_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod3)]) {
        state_.modifiers |= MOD3_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod4)]) {
        state_.modifiers |= MOD4_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Mod5)]) {
        state_.modifiers |= MOD5_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Alt)]) {
        state_.modifiers |= ALT_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Meta)]) {
        state_.modifiers |= META_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Super)]) {
        state_.modifiers |= SUPER_MASK;
    }
    if (mask & modifierMask_[static_cast<uint8_t>(Modifiers::Hyper)]) {
        state_.modifiers |= HYPER_MASK;
    }
}

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

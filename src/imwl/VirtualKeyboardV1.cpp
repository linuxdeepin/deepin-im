// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardV1.h"

#include "DimTextInputV1.h"
#include "Seat.h"
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

VirtualKeyboardV1::VirtualKeyboardV1(Seat *seat)
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
}

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_key(wl::server::Resource *resource,
                                                    uint32_t time,
                                                    uint32_t key,
                                                    uint32_t state)
{
    xkb_keysym_t sym = xkb_state_key_get_one_sym(xkbState_.get(), key);
    auto dti = seat_->getDimTextInputV1();
    dti->sendKeysym(nextSerial(), time, sym, state, 0);
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
}

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

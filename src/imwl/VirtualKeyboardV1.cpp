// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardV1.h"

#include "Seat.h"
#include "wl/server/Resource.h"

#include <xkbcommon/xkbcommon.h>

#include <sys/mman.h>

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
    // q->seat_->sendKeyEvent(key, state);
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
    // wl_resource_destroy(resource->handle);
}

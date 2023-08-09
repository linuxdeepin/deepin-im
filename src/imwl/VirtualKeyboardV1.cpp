// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardV1.h"

#include "wl/server/Resource.h"
#include "wl/server/Seat.h"

#include <xkbcommon/xkbcommon.h>

#include <sys/mman.h>

VirtualKeyboardV1::VirtualKeyboardV1(struct ::wl_resource *seat)
    : seat_(wl::server::Seat::fromResource(seat))
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

    m_xkbKeymap.reset(xkb_keymap_new_from_string(m_xkbContext.get(),
                                                 static_cast<const char *>(ptr),
                                                 XKB_KEYMAP_FORMAT_TEXT_V1,
                                                 XKB_KEYMAP_COMPILE_NO_FLAGS));
    munmap(ptr, size);

    if (!m_xkbKeymap) {
        return;
    }

    m_xkbState.reset(xkb_state_new(m_xkbKeymap.get()));
    if (!m_xkbState) {
        m_xkbKeymap.reset();
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
}

void VirtualKeyboardV1::zwp_virtual_keyboard_v1_destroy(wl::server::Resource *resource)
{
    // wl_resource_destroy(resource->handle);
}

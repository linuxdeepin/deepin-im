// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALKEYBOARDV1_H
#define VIRTUALKEYBOARDV1_H

#include "wl/server/ZwpVirtualKeyboardV1.h"

#include <xkbcommon/xkbcommon.h>

#include <memory>

class QWaylandSeat;

class State
{
public:
    uint32_t serial = 1;
    uint32_t modifiers = 0;
    uint32_t group = 0;
};

class VirtualKeyboardV1 : public wl::server::ZwpVirtualKeyboardV1
{
public:
    VirtualKeyboardV1(QWaylandSeat *seat);
    ~VirtualKeyboardV1();

protected:
    void zwp_virtual_keyboard_v1_keymap(wl::server::Resource *resource,
                                        uint32_t format,
                                        int32_t fd,
                                        uint32_t size) override;
    void zwp_virtual_keyboard_v1_key(wl::server::Resource *resource,
                                     uint32_t time,
                                     uint32_t key,
                                     uint32_t state) override;
    void zwp_virtual_keyboard_v1_modifiers(wl::server::Resource *resource,
                                           uint32_t mods_depressed,
                                           uint32_t mods_latched,
                                           uint32_t mods_locked,
                                           uint32_t group) override;
    void zwp_virtual_keyboard_v1_destroy(wl::server::Resource *resource) override;

private:
    QWaylandSeat *seat_;
    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> xkbContext_;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> xkbKeymap_;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> xkbState_;

    uint32_t modifierMask_[static_cast<uint8_t>(Modifiers::CNT)];
    State state_;
};

#endif // !VIRTUALKEYBOARDV1_H

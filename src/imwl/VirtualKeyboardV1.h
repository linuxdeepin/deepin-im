// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALKEYBOARDV1_H
#define VIRTUALKEYBOARDV1_H

#include "wl/server/ZwpVirtualKeyboardV1.h"

#include <xkbcommon/xkbcommon.h>

#include <memory>

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class VirtualKeyboardV1 : public wl::server::ZwpVirtualKeyboardV1
{
public:
    VirtualKeyboardV1(struct ::wl_resource *seat);
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
    wl::server::Seat *seat_;
    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> m_xkbContext;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> m_xkbKeymap;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> m_xkbState;
};

#endif // !VIRTUALKEYBOARDV1_H

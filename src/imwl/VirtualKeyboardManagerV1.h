// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALKEYBOARDMANAGERV1_H
#define VIRTUALKEYBOARDMANAGERV1_H

#include "wl/server/ZwpVirtualKeyboardManagerV1.h"

#include <memory>
#include <unordered_map>

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class VirtualKeyboardV1;

class VirtualKeyboardManagerV1 : public wl::server::ZwpVirtualKeyboardManagerV1
{
public:
    VirtualKeyboardManagerV1();
    ~VirtualKeyboardManagerV1();

protected:
    void zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(wl::server::Resource *resource,
                                                                 struct ::wl_resource *seat,
                                                                 uint32_t id) override;
};

#endif // !VIRTUALKEYBOARDMANAGERV1_H

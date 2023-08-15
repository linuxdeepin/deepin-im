// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPVIRTUALKEYBOARDMANAGERV1_H
#define WL_SERVER_ZWPVIRTUALKEYBOARDMANAGERV1_H

#include "Type.h"
#include "wayland-virtual-keyboard-unstable-v1-server-protocol.h"

#include <memory>
#include <unordered_map>

namespace wl {
namespace server {

class ZwpVirtualKeyboardV1;

class ZwpVirtualKeyboardManagerV1
    : public Type<ZwpVirtualKeyboardManagerV1, zwp_virtual_keyboard_manager_v1>
{
    friend class Type;

public:
    ZwpVirtualKeyboardManagerV1();
    ~ZwpVirtualKeyboardManagerV1();

protected:
    virtual void zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(Resource *resource, struct ::wl_resource *seat, uint32_t id) = 0;

private:
    static const struct zwp_virtual_keyboard_manager_v1_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPVIRTUALKEYBOARDMANAGERV1_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpVirtualKeyboardManagerV1.h"

using namespace wl::server;

const struct zwp_virtual_keyboard_manager_v1_interface ZwpVirtualKeyboardManagerV1::impl = {
    ResourceCallbackWrapper<&ZwpVirtualKeyboardManagerV1::createVirtualKeyboard>::func,
};

ZwpVirtualKeyboardManagerV1::ZwpVirtualKeyboardManagerV1()
    : Type()
{
}

ZwpVirtualKeyboardManagerV1::~ZwpVirtualKeyboardManagerV1() { }

void ZwpVirtualKeyboardManagerV1::createVirtualKeyboard(struct wl_client *client,
                                                        struct wl_resource *seat,
                                                        uint32_t id)
{
    // zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(client, seat, id);
}

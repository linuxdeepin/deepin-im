// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpVirtualKeyboardV1.h"

using namespace wl::server;

const struct zwp_virtual_keyboard_v1_interface ZwpVirtualKeyboardV1::impl = {
    ResourceCallbackWrapper<&ZwpVirtualKeyboardV1::zwp_virtual_keyboard_v1_keymap>::func,
    ResourceCallbackWrapper<&ZwpVirtualKeyboardV1::zwp_virtual_keyboard_v1_key>::func,
    ResourceCallbackWrapper<&ZwpVirtualKeyboardV1::zwp_virtual_keyboard_v1_modifiers>::func,
    ResourceCallbackWrapper<&ZwpVirtualKeyboardV1::zwp_virtual_keyboard_v1_destroy>::func,
};

ZwpVirtualKeyboardV1::ZwpVirtualKeyboardV1() { }

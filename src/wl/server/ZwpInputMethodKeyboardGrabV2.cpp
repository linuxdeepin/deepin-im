// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodKeyboardGrabV2.h"

using namespace wl::server;

const struct zwp_input_method_keyboard_grab_v2_interface ZwpInputMethodKeyboardGrabV2::impl = {
    ResourceCallbackWrapper<&ZwpInputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_release>::func,
};

ZwpInputMethodKeyboardGrabV2::ZwpInputMethodKeyboardGrabV2()
    : Type()
{
}

ZwpInputMethodKeyboardGrabV2::~ZwpInputMethodKeyboardGrabV2() { }

void ZwpInputMethodKeyboardGrabV2::send_keymap(struct wl_resource *resource, uint32_t format, int32_t fd, uint32_t size) {
    zwp_input_method_keyboard_grab_v2_send_keymap(resource, format, fd, size);
}

void ZwpInputMethodKeyboardGrabV2::send_key(
    struct wl_resource *resource, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
    zwp_input_method_keyboard_grab_v2_send_key(resource, serial, time, key, state);
}


void ZwpInputMethodKeyboardGrabV2::send_modifiers(struct wl_resource *resource, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    zwp_input_method_keyboard_grab_v2_send_modifiers(resource, serial, mods_depressed, mods_latched, mods_locked, group);
}

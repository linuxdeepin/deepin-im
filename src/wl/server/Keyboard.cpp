// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keyboard.h"

using namespace wl::server;

const struct wl_keyboard_interface Keyboard::impl = {
    ResourceCallbackWrapper<&Keyboard::wl_keyboard_release>::func,
};

Keyboard::Keyboard() {
}

Keyboard::~Keyboard() {}

void Keyboard::send_keymap(struct wl_resource *resource_, uint32_t format, int32_t fd, uint32_t size) {
    wl_keyboard_send_keymap(resource_, format, fd, size);
}

void Keyboard::send_enter(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface, struct wl_array *keys) {
    wl_keyboard_send_enter(resource_, serial, surface, keys);
}

void Keyboard::send_leave(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface) {
    wl_keyboard_send_leave(resource_, serial, surface);
}

void Keyboard::send_key(struct wl_resource *resource_, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
    wl_keyboard_send_key(resource_, serial, time, key, state);
}

void Keyboard::send_modifiers(struct wl_resource *resource_, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    wl_keyboard_send_modifiers(resource_, serial, mods_depressed, mods_latched, mods_locked, group);
}

void Keyboard::send_repeat_info(struct wl_resource *resource_, int32_t rate, int32_t delay) {
    wl_keyboard_send_repeat_info(resource_, rate, delay);
}

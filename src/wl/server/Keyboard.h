// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_KEYBOARD_H
#define WL_SERVER_KEYBOARD_H

#include "Type.h"

#include <wayland-server-protocol.h>

namespace wl {
namespace server {

class Resource;

class Keyboard : public Type<Keyboard, wl_keyboard>
{
    friend class Type;

public:
    Keyboard();
    virtual ~Keyboard();

    void send_keymap(struct wl_resource *resource_, uint32_t format, int32_t fd, uint32_t size);
    void send_enter(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface, struct wl_array *keys);
    void send_leave(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface);
    void send_key(struct wl_resource *resource_, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
    void send_modifiers(struct wl_resource *resource_, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
    void send_repeat_info(struct wl_resource *resource_, int32_t rate, int32_t delay);

protected:
    virtual void wl_keyboard_release(Resource *resource) = 0;

private:
    static const struct wl_keyboard_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_KEYBOARD_H

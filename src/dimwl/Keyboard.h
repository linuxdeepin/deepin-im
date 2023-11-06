// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYBAORD_H
#define KEYBAORD_H

#include <wayland-server-core.h>

struct wlr_seat;
struct wlr_input_device;
struct wlr_keyboard;

class Server;

class Keyboard
{
public:
    Keyboard(Server *server, wlr_seat *seat, wlr_input_device *device, wl_list *list);
    ~Keyboard();

private:
    static void modifiersNotify(struct wl_listener *listener, void *data);
    static void keyNotify(struct wl_listener *listener, void *data);
    static void destroyNotify(struct wl_listener *listener, void *data);

private:
    wl_list link_;

    Server *server_;
    wlr_keyboard *wlr_keyboard_;

    wl_listener modifiers_;
    wl_listener key_;
    wl_listener destroy_;
};

#endif // !KEYBAORD_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYBAORD_H
#define KEYBAORD_H

#include "Listener.h"

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
    void modifiersNotify(void *data);
    void keyNotify(void *data);
    void destroyNotify(void *data);

private:
    wl_list link_;

    Server *server_;
    wlr_keyboard *wlr_keyboard_;

    Listener<&Keyboard::modifiersNotify> modifiers_;
    Listener<&Keyboard::keyNotify> key_;
    Listener<&Keyboard::destroyNotify> destroy_;
};

#endif // !KEYBAORD_H

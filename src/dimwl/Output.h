// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OUTPUT_H
#define OUTPUT_H

#include <wayland-server-core.h>
#include <wayland-util.h>

struct wlr_output;

class Server;

class Output
{
public:
    Output(Server *server, struct wlr_output *wlr_output, wl_list *list);
    ~Output();

private:
    static void frameNotify(struct wl_listener *listener, void *data);
    static void destroyNotify(struct wl_listener *listener, void *data);

private:
    wl_list link_;

    Server *server_;
    struct wlr_output *wlr_output_;

    wl_listener frame_;
    wl_listener destroy_;
};

#endif // !OUTPUT_H

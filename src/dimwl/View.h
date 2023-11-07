// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIEW_H
#define VIEW_H

#include "Listener.h"

extern "C" {
#include <wlr/backend.h>
#define static
#include <wlr/types/wlr_scene.h>
#undef static
#include <wlr/types/wlr_xdg_shell.h>
}

class Server;

class View
{
public:
    View(Server *server, wlr_xdg_surface *xdg_surface, wl_list *list);
    ~View();

private:
    void mapNotify(void *data);
    void unmapNotify(void *data);
    void destroyNotify(void *data);
    void xdgToplevelRequestMoveNotify(void *data);
    void xdgToplevelRequestResizeNotify(void *data);
    void xdgToplevelRequestMaximizeNotify(void *data);
    void xdgToplevelRequestFullscreenNotify(void *data);

    void focusView();

private:
    wl_list link_;
    wl_list *parentLink_;

    Server *server_;
    wlr_xdg_toplevel *xdg_toplevel_;
    wlr_scene_tree *scene_tree_;

    Listener<&View::mapNotify> map_;
    Listener<&View::unmapNotify> unmap_;
    Listener<&View::destroyNotify> destroy_;
    Listener<&View::xdgToplevelRequestMoveNotify> request_move_;
    Listener<&View::xdgToplevelRequestResizeNotify> request_resize_;
    Listener<&View::xdgToplevelRequestMaximizeNotify> request_maximize_;
    Listener<&View::xdgToplevelRequestFullscreenNotify> request_fullscreen_;
};

#endif // !VIEW_H

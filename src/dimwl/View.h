#ifndef VIEW_H
#define VIEW_H

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
    View(Server *server, wlr_xdg_surface *xdg_surface, wl_list *parentLink);
    ~View();

private:
    static void mapNotify(struct wl_listener *listener, void *data);
    static void unmapNotify(struct wl_listener *listener, void *data);
    static void destroyNotify(struct wl_listener *listener, void *data);

private:
    wl_list link_;
    wl_list *parentLink_;

    Server *server_;
    wlr_xdg_toplevel *xdg_toplevel_;
    wlr_scene_tree *scene_tree_;

    wl_listener map_;
    wl_listener unmap_;
    wl_listener destroy_;
};

#endif // !VIEW_H

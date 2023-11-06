#include "View.h"

#include "Server.h"

View::View(Server *server, wlr_xdg_surface *xdg_surface, wl_list *parentLink)
    : parentLink_(parentLink)
    , server_(server)
{
    xdg_toplevel_ = xdg_surface->toplevel;
    scene_tree_ = wlr_scene_xdg_surface_create(&server_->scene()->tree, xdg_toplevel_->base);
    scene_tree_->node.data = this;
    xdg_surface->data = scene_tree_;

    /* Listen to the various events it can emit */
    map_.notify = mapNotify;
    wl_signal_add(&xdg_surface->events.map, &map_);
    unmap_.notify = unmapNotify;
    wl_signal_add(&xdg_surface->events.unmap, &unmap_);
    destroy_.notify = destroyNotify;
    wl_signal_add(&xdg_surface->events.destroy, &destroy_);
}

View::~View()
{
    wl_list_remove(&map_.link);
    wl_list_remove(&unmap_.link);
    wl_list_remove(&destroy_.link);
    // wl_list_remove(&request_move.link);
    // wl_list_remove(&request_resize.link);
    // wl_list_remove(&request_maximize.link);
    // wl_list_remove(&request_fullscreen.link);
}

void View::mapNotify(struct wl_listener *listener, void *data)
{
    /* Called when the surface is mapped, or ready to display on-screen. */
    View *view = wl_container_of(listener, view, map_);

    wl_list_insert(view->parentLink_, &view->link_);

    // focus_view(view, view->xdg_toplevel_->base->surface);
}

void View::unmapNotify(struct wl_listener *listener, void *data)
{
    /* Called when the surface is unmapped, and should no longer be shown. */
    View *view = wl_container_of(listener, view, unmap_);

    /* Reset the cursor mode if the grabbed view was unmapped. */
    // if (view == view->server->grabbed_view) {
    //     reset_cursor_mode(view->server);
    // }

    wl_list_remove(&view->link_);
}

void View::destroyNotify(struct wl_listener *listener, void *data)
{
    /* Called when the surface is destroyed and should never be shown again. */
    View *view = wl_container_of(listener, view, destroy_);
    delete view;
}

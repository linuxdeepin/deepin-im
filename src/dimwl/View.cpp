#include "View.h"

#include "Server.h"

View::View(Server *server, wlr_xdg_surface *xdg_surface, wl_list *parentLink)
    : parentLink_(parentLink)
    , server_(server)
    , map_(this)
    , unmap_(this)
    , destroy_(this)
{
    xdg_toplevel_ = xdg_surface->toplevel;
    scene_tree_ = wlr_scene_xdg_surface_create(&server_->scene()->tree, xdg_toplevel_->base);
    scene_tree_->node.data = this;
    xdg_surface->data = scene_tree_;

    /* Listen to the various events it can emit */
    wl_signal_add(&xdg_surface->events.map, map_);
    wl_signal_add(&xdg_surface->events.unmap, unmap_);
    wl_signal_add(&xdg_surface->events.destroy, destroy_);
}

View::~View()
{
    // wl_list_remove(&request_move.link);
    // wl_list_remove(&request_resize.link);
    // wl_list_remove(&request_maximize.link);
    // wl_list_remove(&request_fullscreen.link);
}

void View::mapNotify(void *data)
{
    /* Called when the surface is mapped, or ready to display on-screen. */

    wl_list_insert(parentLink_, &link_);

    // focus_view(view, view->xdg_toplevel_->base->surface);
}

void View::unmapNotify(void *data)
{
    /* Called when the surface is unmapped, and should no longer be shown. */

    /* Reset the cursor mode if the grabbed view was unmapped. */
    // if (view == view->server->grabbed_view) {
    //     reset_cursor_mode(view->server);
    // }

    wl_list_remove(&link_);
}

void View::destroyNotify(void *data)
{
    /* Called when the surface is destroyed and should never be shown again. */
    delete this;
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Server.h"

#include "Keyboard.h"
#include "Output.h"
#include "View.h"

extern "C" {
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_subcompositor.h>
}

#include <stdexcept>

#include <assert.h>

Server::Server()
    : display_(wl_display_create())
    , new_output_(this)
    , new_xdg_surface_(this)
    , cursor_motion_(this)
    , cursor_motion_absolute_(this)
    , cursor_button_(this)
    , cursor_axis_(this)
    , cursor_frame_(this)
    , seat_request_cursor_(this)
    , seat_request_set_selection_(this)
    , new_input_(this)
{
    backend_.reset(wlr_backend_autocreate(display_.get()));
    if (!backend_) {
        throw std::runtime_error("failed to create xkb context");
    }

    renderer_.reset(wlr_renderer_autocreate(backend_.get()));
    if (!renderer_) {
        throw std::runtime_error("failed to create wlr_renderer");
    }

    wlr_renderer_init_wl_display(renderer_.get(), display_.get());

    allocator_.reset(wlr_allocator_autocreate(backend_.get(), renderer_.get()));
    if (!allocator_) {
        throw std::runtime_error("failed to create wlr_allocator");
    }

    /* This creates some hands-off wlroots interfaces. The compositor is
     * necessary for clients to allocate surfaces, the subcompositor allows to
     * assign the role of subsurfaces to surfaces and the data device manager
     * handles the clipboard. Each of these wlroots interfaces has room for you
     * to dig your fingers in and play with their behavior if you want. Note that
     * the clients cannot set the selection directly without compositor approval,
     * see the handling of the request_set_selection event below.*/
    wlr_compositor_create(display_.get(), renderer_.get());
    wlr_subcompositor_create(display_.get());
    wlr_data_device_manager_create(display_.get());

    /* Creates an output layout, which a wlroots utility for working with an
     * arrangement of screens in a physical layout. */
    output_layout_.reset(wlr_output_layout_create());

    /* Configure a listener to be notified when new outputs are available on the
     * backend. */
    wl_list_init(&outputs_);
    wl_signal_add(&backend_->events.new_output, new_output_);

    /* Create a scene graph. This is a wlroots abstraction that handles all
     * rendering and damage tracking. All the compositor author needs to do
     * is add things that should be rendered to the scene graph at the proper
     * positions and then call wlr_scene_output_commit() to render a frame if
     * necessary.
     */
    scene_.reset(wlr_scene_create());
    wlr_scene_attach_output_layout(scene_.get(), output_layout_.get());

    /* Set up xdg-shell version 3. The xdg-shell is a Wayland protocol which is
     * used for application windows. For more detail on shells, refer to my
     * article:
     *
     * https://drewdevault.com/2018/07/29/Wayland-shells.html
     */
    wl_list_init(&views_);
    xdg_shell_.reset(wlr_xdg_shell_create(display_.get(), 3));
    wl_signal_add(&xdg_shell_->events.new_surface, new_xdg_surface_);

    /*
     * Creates a cursor, which is a wlroots utility for tracking the cursor
     * image shown on screen.
     */
    cursor_.reset(wlr_cursor_create());
    wlr_cursor_attach_output_layout(cursor_.get(), output_layout_.get());

    /* Creates an xcursor manager, another wlroots utility which loads up
     * Xcursor themes to source cursor images from and makes sure that cursor
     * images are available at all scale factors on the screen (necessary for
     * HiDPI support). We add a cursor theme at scale factor 1 to begin with. */
    cursor_mgr_.reset(wlr_xcursor_manager_create(nullptr, 24));
    wlr_xcursor_manager_load(cursor_mgr_.get(), 1);

    /*
     * wlr_cursor *only* displays an image on screen. It does not move around
     * when the pointer moves. However, we can attach input devices to it, and
     * it will generate aggregate events for all of them. In these events, we
     * can choose how we want to process them, forwarding them to clients and
     * moving the cursor around. More detail on this process is described in my
     * input handling blog post:
     *
     * https://drewdevault.com/2018/07/17/Input-handling-in-wlroots.html
     *
     * And more comments are sprinkled throughout the notify functions above.
     */
    // server.cursor_mode = TINYWL_CURSOR_PASSTHROUGH;
    wl_signal_add(&cursor_->events.motion, cursor_motion_);
    wl_signal_add(&cursor_->events.motion_absolute, cursor_motion_absolute_);
    wl_signal_add(&cursor_->events.button, cursor_button_);
    wl_signal_add(&cursor_->events.axis, cursor_axis_);
    wl_signal_add(&cursor_->events.frame, cursor_frame_);

    /*
     * Configures a seat, which is a single "seat" at which a user sits and
     * operates the computer. This conceptually includes up to one keyboard,
     * pointer, touch, and drawing tablet device. We also rig up a listener to
     * let us know when new input devices are available on the backend.
     */
    seat_.reset(wlr_seat_create(display_.get(), "seat0"));
    wl_signal_add(&seat_->events.request_set_cursor, seat_request_cursor_);
    wl_signal_add(&seat_->events.request_set_selection, seat_request_set_selection_);
    wl_list_init(&keyboards_);
    wl_signal_add(&backend_->events.new_input, new_input_);
}

Server::~Server() = default;

std::string Server::addSocketAuto()
{
    return wl_display_add_socket_auto(display_.get());
}

void Server::run()
{
    wl_display_run(display_.get());
}

bool Server::startBackend()
{
    return wlr_backend_start(backend_.get());
}

void Server::newOutputNotify(void *data)
{
    /* This event is raised by the backend when a new output (aka a display or
     * monitor) becomes available. */
    struct wlr_output *wlr_output = static_cast<struct wlr_output *>(data);

    if (!wl_list_empty(&wlr_output->modes)) {
        struct wlr_output_mode *mode = wl_container_of(wlr_output->modes.prev, mode, link);
        wlr_output_set_mode(wlr_output, mode);
    }

    auto *output = new Output(this, wlr_output, &outputs_);
}

void Server::newXdgSurfaceNotify(void *data)
{
    /* This event is raised when wlr_xdg_shell receives a new xdg surface from a
     * client, either a toplevel (application window) or popup. */
    struct wlr_xdg_surface *xdg_surface = static_cast<wlr_xdg_surface *>(data);

    /* We must add xdg popups to the scene graph so they get rendered. The
     * wlroots scene graph provides a helper for this, but to use it we must
     * provide the proper parent scene node of the xdg popup. To enable this,
     * we always set the user data field of xdg_surfaces to the corresponding
     * scene node. */
    if (xdg_surface->role == WLR_XDG_SURFACE_ROLE_POPUP) {
        struct wlr_xdg_surface *parent =
            wlr_xdg_surface_from_wlr_surface(xdg_surface->popup->parent);
        struct wlr_scene_tree *parent_tree = static_cast<wlr_scene_tree *>(parent->data);
        xdg_surface->data = wlr_scene_xdg_surface_create(parent_tree, xdg_surface);
        return;
    }
    assert(xdg_surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);

    new View(this, xdg_surface, &views_);
}

void Server::cursorMotionNotify(void *data)
{
    /* This event is forwarded by the cursor when a pointer emits a _relative_
     * pointer motion event (i.e. a delta) */
    struct wlr_pointer_motion_event *event = static_cast<wlr_pointer_motion_event *>(data);
    /* The cursor doesn't move unless we tell it to. The cursor automatically
     * handles constraining the motion to the output layout, as well as any
     * special configuration applied for the specific input device which
     * generated the event. You can pass NULL for the device if you want to move
     * the cursor around without any input. */
    wlr_cursor_move(cursor_.get(), &event->pointer->base, event->delta_x, event->delta_y);
    // process_cursor_motion(server, event->time_msec);
}

void Server::cursorMotionAbsoluteNotify(void *data)
{
    /* This event is forwarded by the cursor when a pointer emits an _absolute_
     * motion event, from 0..1 on each axis. This happens, for example, when
     * wlroots is running under a Wayland window rather than KMS+DRM, and you
     * move the mouse over the window. You could enter the window from any edge,
     * so we have to warp the mouse there. There is also some hardware which
     * emits these events. */
    struct wlr_pointer_motion_absolute_event *event =
        static_cast<wlr_pointer_motion_absolute_event *>(data);
    wlr_cursor_warp_absolute(cursor_.get(), &event->pointer->base, event->x, event->y);
    processCursorMotion(event->time_msec);
}

void Server::cursorButtonNotify(void *data)
{
    /* This event is forwarded by the cursor when a pointer emits a button
     * event. */
    struct wlr_pointer_button_event *event = static_cast<wlr_pointer_button_event *>(data);
    /* Notify the client with pointer focus that a button press has occurred */
    wlr_seat_pointer_notify_button(seat_.get(), event->time_msec, event->button, event->state);
    double sx, sy;
    struct wlr_surface *surface = NULL;
    View *view = desktopViewAt(cursor_->x, cursor_->y, &surface, &sx, &sy);
    if (event->state == WLR_BUTTON_RELEASED) {
        /* If you released any buttons, we exit interactive move/resize mode. */
        // reset_cursor_mode(server);
    } else {
        /* Focus that client if the button was _pressed_ */
        // focus_view(view, surface);
    }
}

void Server::cursorAxisNotify(void *data)
{
    /* This event is forwarded by the cursor when a pointer emits an axis event,
     * for example when you move the scroll wheel. */
    struct wlr_pointer_axis_event *event = static_cast<wlr_pointer_axis_event *>(data);
    /* Notify the client with pointer focus of the axis event. */
    wlr_seat_pointer_notify_axis(seat_.get(),
                                 event->time_msec,
                                 event->orientation,
                                 event->delta,
                                 event->delta_discrete,
                                 event->source);
}

void Server::cursorFrameNotify(void *data)
{
    /* This event is forwarded by the cursor when a pointer emits an frame
     * event. Frame events are sent after regular pointer events to group
     * multiple events together. For instance, two axis events may happen at the
     * same time, in which case a frame event won't be sent in between. */
    /* Notify the client with pointer focus of the frame event. */
    wlr_seat_pointer_notify_frame(seat_.get());
}

void Server::newInputNotify(void *data)
{
    /* This event is raised by the backend when a new input device becomes
     * available. */
    struct wlr_input_device *device = static_cast<wlr_input_device *>(data);
    switch (device->type) {
    case WLR_INPUT_DEVICE_KEYBOARD:
        new Keyboard(this, seat_.get(), device, &keyboards_);
        break;
    case WLR_INPUT_DEVICE_POINTER:
        /* We don't do anything special with pointers. All of our pointer handling
         * is proxied through wlr_cursor. On another compositor, you might take this
         * opportunity to do libinput configuration on the device to set
         * acceleration, etc. */
        wlr_cursor_attach_input_device(cursor_.get(), device);
        break;
    default:
        break;
    }
    /* We need to let the wlr_seat know what our capabilities are, which is
     * communiciated to the client. In TinyWL we always have a cursor, even if
     * there are no pointer devices, so we always include that capability. */
    uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
    if (!wl_list_empty(&keyboards_)) {
        caps |= WL_SEAT_CAPABILITY_KEYBOARD;
    }
    wlr_seat_set_capabilities(seat_.get(), caps);
}

void Server::seatRequestCursorNotify(void *data) { }

void Server::seatRequestSetSelectionNotify(void *data) { }

void Server::processCursorMotion(uint32_t time)
{
    /* If the mode is non-passthrough, delegate to those functions. */
    // if (cursor_mode == TINYWL_CURSOR_MOVE) {
    //     process_cursor_move(server, time);
    //     return;
    // } else if (server->cursor_mode == TINYWL_CURSOR_RESIZE) {
    //     process_cursor_resize(server, time);
    //     return;
    // }

    /* Otherwise, find the view under the pointer and send the event along. */
    double sx, sy;
    struct wlr_seat *seat = seat_.get();
    struct wlr_surface *surface = NULL;
    View *view = desktopViewAt(cursor_->x, cursor_->y, &surface, &sx, &sy);
    if (!view) {
        /* If there's no view under the cursor, set the cursor image to a
         * default. This is what makes the cursor image appear when you move it
         * around the screen, not over any views. */
        wlr_xcursor_manager_set_cursor_image(cursor_mgr_.get(), "left_ptr", cursor_.get());
    }
    if (surface) {
        /*
         * Send pointer enter and motion events.
         *
         * The enter event gives the surface "pointer focus", which is distinct
         * from keyboard focus. You get pointer focus by moving the pointer over
         * a window.
         *
         * Note that wlroots will avoid sending duplicate enter/motion events if
         * the surface has already has pointer focus or if the client is already
         * aware of the coordinates passed.
         */
        wlr_seat_pointer_notify_enter(seat, surface, sx, sy);
        wlr_seat_pointer_notify_motion(seat, time, sx, sy);
    } else {
        /* Clear pointer focus so future button events and such are not sent to
         * the last client to have the cursor over it. */
        wlr_seat_pointer_clear_focus(seat);
    }
}

View *
Server::desktopViewAt(double lx, double ly, struct wlr_surface **surface, double *sx, double *sy)
{
    /* This returns the topmost node in the scene at the given layout coords.
     * we only care about surface nodes as we are specifically looking for a
     * surface in the surface tree of a tinywl_view. */
    struct wlr_scene_node *node = wlr_scene_node_at(&scene_->tree.node, lx, ly, sx, sy);
    if (node == NULL || node->type != WLR_SCENE_NODE_BUFFER) {
        return NULL;
    }
    struct wlr_scene_buffer *scene_buffer = wlr_scene_buffer_from_node(node);
    struct wlr_scene_surface *scene_surface = wlr_scene_surface_from_buffer(scene_buffer);
    if (!scene_surface) {
        return NULL;
    }

    *surface = scene_surface->surface;
    /* Find the node corresponding to the tinywl_view at the root of this
     * surface tree, it is the only one for which we set the data field. */
    struct wlr_scene_tree *tree = node->parent;
    while (tree != NULL && tree->node.data == NULL) {
        tree = tree->node.parent;
    }
    return static_cast<View *>(tree->node.data);
}

#ifndef SERVER_H
#define SERVER_H

#include "common/common.h"

#include <wayland-server-core.h>

#include <string>

extern "C" {
#include <wlr/backend.h>
#define static
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#undef static
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_shell.h>
}

#include <memory>

class View;

class Server
{
public:
    Server();
    ~Server();

    std::string addSocketAuto();
    void run();

    bool startBackend();

    wlr_allocator *allocator() { return allocator_.get(); }

    wlr_renderer *renderer() { return renderer_.get(); }

    wlr_output_layout *outputLayout() { return output_layout_.get(); }

    wlr_scene *scene() { return scene_.get(); }

    wlr_seat *seat() { return seat_.get(); }

private:
    static void newOutputNotify(struct wl_listener *listener, void *data);
    static void newXdgSurfaceNotify(struct wl_listener *listener, void *data);
    static void cursorMotionNotify(struct wl_listener *listener, void *data);
    static void cursorMotionAbsoluteNotify(struct wl_listener *listener, void *data);
    static void cursorButtonNotify(struct wl_listener *listener, void *data);
    static void cursorAxisNotify(struct wl_listener *listener, void *data);
    static void cursorFrameNotify(struct wl_listener *listener, void *data);
    static void newInputNotify(struct wl_listener *listener, void *data);
    static void seatRequestCursorNotify(struct wl_listener *listener, void *data);
    static void seatRequestSetSelectionNotify(struct wl_listener *listener, void *data);

    void processCursorMotion(uint32_t time);
    View *desktopViewAt(double lx, double ly, struct wlr_surface **surface, double *sx, double *sy);

private:
    std::unique_ptr<wl_display, Deleter<wl_display_destroy>> display_;
    std::unique_ptr<wlr_backend, Deleter<wlr_backend_destroy>> backend_;
    std::unique_ptr<wlr_renderer, Deleter<wlr_renderer_destroy>> renderer_;
    std::unique_ptr<wlr_allocator, Deleter<wlr_allocator_destroy>> allocator_;
    std::unique_ptr<wlr_output_layout, Deleter<wlr_output_layout_destroy>> output_layout_;
    wl_listener new_output_;
    wl_list outputs_;
    std::unique_ptr<wlr_scene> scene_;
    wl_list views_;
    std::unique_ptr<wlr_xdg_shell> xdg_shell_;
    wl_listener new_xdg_surface_;
    std::unique_ptr<wlr_cursor, Deleter<wlr_cursor_destroy>> cursor_;
    std::unique_ptr<wlr_xcursor_manager, Deleter<wlr_xcursor_manager_destroy>> cursor_mgr_;
    wl_listener cursor_motion_;
    wl_listener cursor_motion_absolute_;
    wl_listener cursor_button_;
    wl_listener cursor_axis_;
    wl_listener cursor_frame_;
    wl_list keyboards_;
    wl_listener new_input_;
    std::unique_ptr<wlr_seat, Deleter<wlr_seat_destroy>> seat_;
    wl_listener request_cursor_;
    wl_listener request_set_selection_;
};

#endif // !SERVER_H

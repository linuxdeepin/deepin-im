// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERVER_H
#define SERVER_H

#include "Listener.h"
#include "common/common.h"

#include <wayland-server-core.h>

#include <functional>
#include <string>

extern "C" {
#include <wlr/backend.h>
#define static
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_output_layout.h>
#undef static
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_xcursor_manager.h>
}

#include <memory>

struct wlr_scene;
struct wlr_xdg_shell;
struct wlr_virtual_keyboard_manager_v1;
struct wlr_input_method_manager_v2;

class Output;
class View;
class Keyboard;
class InputMethodV2;

class Server
{
public:
    Server(const std::shared_ptr<wl_display> &local, const std::shared_ptr<wlr_backend> &backend);
    ~Server();

    std::string addSocketAuto();
    bool addSocket(const std::string &name);
    int getFd();
    wl_event_loop *getEventLoop();
    void flushClients();
    void run();

    void setVirtualKeyboardCallback(const std::function<void(Keyboard *)> &callback)
    {
        virtualKeyboardCallback_ = callback;
    }

    void setInputMethodCallback(const std::function<void()> &callback)
    {
        inputMethodCallback_ = callback;
    }

    void createOutput();
    void createOutputFromSurface(wl_surface *surface);

    wlr_allocator *allocator() { return allocator_.get(); }

    wlr_renderer *renderer() { return renderer_.get(); }

    wlr_scene *scene() { return scene_.get(); }

    Output *output() { return output_; }

    wl_list *views() { return &views_; }

    wlr_seat *seat() { return seat_.get(); }

    InputMethodV2 *inputMethod() { return input_method_; }

private:
    void backendNewOutputNotify(void *data);
    void compositorNewSurfaceNotify(void *data);
    void xdgShellNewSurfaceNotify(void *data);
    void backendNewInputNotify(void *data);
    void outputPresentNotify(void *data);
    void seatRequestCursorNotify(void *data);
    void seatRequestSetSelectionNotify(void *data);
    void keyboardKeyNotify(void *data);
    void virtualKeyboardManagerNewVirtualKeyboardNotify(void *data);
    void inputMethodManagerV2InputMethodNotify(void *data);
    void inputMethodV2DestroyNotify(void *data);

    View *desktopViewAt(double lx, double ly, struct wlr_surface **surface, double *sx, double *sy);

private:
    std::shared_ptr<wl_display> display_;
    std::shared_ptr<wlr_backend> backend_;
    std::unique_ptr<wlr_renderer, Deleter<wlr_renderer_destroy>> renderer_;
    std::unique_ptr<wlr_allocator, Deleter<wlr_allocator_destroy>> allocator_;
    Output *output_ = nullptr;
    Listener<&Server::backendNewOutputNotify> backend_new_output_;
    Listener<&Server::outputPresentNotify> output_present_;

    std::unique_ptr<wlr_scene> scene_;

    Listener<&Server::compositorNewSurfaceNotify> compositor_new_surface_;

    wl_list views_;
    std::unique_ptr<wlr_xdg_shell> xdg_shell_;
    Listener<&Server::xdgShellNewSurfaceNotify> xdg_shell_new_surface_;

    std::unique_ptr<wlr_seat, Deleter<wlr_seat_destroy>> seat_;
    Listener<&Server::seatRequestCursorNotify> seat_request_cursor_;
    Listener<&Server::seatRequestSetSelectionNotify> seat_request_set_selection_;
    wl_list keyboards_;
    Listener<&Server::backendNewInputNotify> backend_new_input_;

    std::unique_ptr<wlr_virtual_keyboard_manager_v1> virtual_keyboard_manager_v1_;
    Listener<&Server::virtualKeyboardManagerNewVirtualKeyboardNotify>
        virtual_keyboard_manager_v1_new_virtual_keyboard_;

    std::unique_ptr<wlr_input_method_manager_v2> input_method_manager_v2_;
    Listener<&Server::inputMethodManagerV2InputMethodNotify> input_method_manager_v2_input_method_;
    InputMethodV2 *input_method_ = nullptr;
    Listener<&Server::inputMethodV2DestroyNotify> input_method_v2_destroy_;

    std::function<void(Keyboard *)> virtualKeyboardCallback_;
    std::function<void()> inputMethodCallback_;
};

#endif // !SERVER_H

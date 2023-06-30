#include "WaylandServer.h"

#include "wayland-input-method-unstable-v1-server-protocol.h"
#include "wayland-text-input-unstable-v3-server-protocol.h"

#include <wayland-server.h>

#include <iostream>
#include <stdexcept>

void text_input_v3_bind([[maybe_unused]] struct wl_client *client,
                        [[maybe_unused]] void *data,
                        uint32_t version,
                        uint32_t id)
{
    std::cout << "text_input_v3_bind:" << version << id << std::endl;
}

WaylandServer::WaylandServer()
    : display_(wl_display_create())
{
    int ret = wl_display_add_socket(display_, "imfakewl");
    if (ret == -1) {
        throw std::runtime_error("Unable to add socket to Wayland display");
    }

    wl_global_create(display_, &zwp_text_input_v3_interface, 1, nullptr, &text_input_v3_bind);
    wl_global_create(display_, &zwp_input_method_v1_interface, 1, nullptr, nullptr);
    wl_global_create(display_, &zwp_input_method_context_v1_interface, 1, nullptr, nullptr);
}

WaylandServer::~WaylandServer()
{
    wl_display_destroy(display_);
}

void WaylandServer::run()
{
    wl_display_run(display_);
}

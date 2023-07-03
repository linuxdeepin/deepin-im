#include "WaylandServer.h"

#include "utils.h"
#include "wayland-input-method-unstable-v1-server-protocol.h"
#include "wayland-text-input-unstable-v3-server-protocol.h"

#include <wayland-server.h>

#include <QDebug>

#include <iostream>
#include <stdexcept>

WaylandServer::WaylandServer()
    : display_(wl_display_create())
{
    int ret = wl_display_add_socket(display_, "imfakewl");
    if (ret == -1) {
        throw std::runtime_error("Unable to add socket to Wayland display");
    }

    wl_global_create(display_,
                     &zwp_text_input_v3_interface,
                     1,
                     this,
                     CallbackWrapper<&WaylandServer::textInputV3Bind>::func);
    wl_global_create(display_,
                     &zwp_input_method_v1_interface,
                     1,
                     this,
                     CallbackWrapper<&WaylandServer::inputMethodV1Bind>::func);
    wl_global_create(display_,
                     &zwp_input_method_context_v1_interface,
                     1,
                     this,
                     CallbackWrapper<&WaylandServer::inputMethodContextV1Bind>::func);
}

WaylandServer::~WaylandServer()
{
    wl_display_destroy(display_);
}

void WaylandServer::run()
{
    wl_display_run(display_);
}

void WaylandServer::textInputV3Bind([[maybe_unused]] struct wl_client *client,
                                    uint32_t version,
                                    uint32_t id)
{
    std::cout << "text_input_v3_bind:" << version << id << std::endl;
}

void WaylandServer::inputMethodV1Bind([[maybe_unused]] struct wl_client *client,
                                      uint32_t version,
                                      uint32_t id)
{
    std::cout << "input_method_v1_bind:" << version << id << std::endl;
}

void WaylandServer::inputMethodContextV1Bind([[maybe_unused]] struct wl_client *client,
                                             uint32_t version,
                                             uint32_t id)
{
    std::cout << "input_method_context_v1_bind:" << version << id << std::endl;
}

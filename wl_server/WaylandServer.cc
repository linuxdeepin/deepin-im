#include "WaylandServer.h"

#include "utils.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"
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

    text_input_manager_global_ =
        wl_global_create(display_,
                         &zwp_text_input_manager_v3_interface,
                         1,
                         this,
                         CallbackWrapper<&WaylandServer::textInputManagerV3Bind>::func);
    text_input_global_ = wl_global_create(display_,
                                          &zwp_text_input_v3_interface,
                                          1,
                                          this,
                                          CallbackWrapper<&WaylandServer::textInputV3Bind>::func);
    input_method_manager_global_ =
        wl_global_create(display_,
                         &zwp_input_method_manager_v2_interface,
                         1,
                         this,
                         CallbackWrapper<&WaylandServer::inputMethodContextV1Bind>::func);
    input_method_global_ =
        wl_global_create(display_,
                         &zwp_input_method_v2_interface,
                         1,
                         this,
                         CallbackWrapper<&WaylandServer::inputMethodV1Bind>::func);
}

WaylandServer::~WaylandServer()
{
    wl_display_destroy(display_);
}

void WaylandServer::run()
{
    wl_display_run(display_);
}

static const struct zwp_text_input_manager_v3_interface
    zwp_text_input_manager_v3_interface_implementation = {
        // todo:
    };

static void
zwp_text_input_manager_v3_resource_destroy([[maybe_unused]] struct wl_resource *resource)
{
    // struct my_output *client_output = wl_resource_get_user_data(resource);

    // // TODO: Clean up resource

    // remove_to_list(client_output->state->client_outputs, client_output);
}

void WaylandServer::textInputManagerV3Bind([[maybe_unused]] struct wl_client *client,
                                           uint32_t version,
                                           uint32_t id)
{
    std::cout << "text_input_manager_v3_bind:" << version << id << std::endl;

    struct wl_resource *resource = wl_resource_create(client,
                                                      &zwp_text_input_manager_v3_interface,
                                                      zwp_text_input_v3_interface.version,
                                                      id);

    wl_resource_set_implementation(resource,
                                   &zwp_text_input_manager_v3_interface_implementation,
                                   this,
                                   &zwp_text_input_manager_v3_resource_destroy);
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

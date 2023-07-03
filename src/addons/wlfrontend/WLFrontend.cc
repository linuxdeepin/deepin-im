#include "WLFrontend.h"

#include "WaylandConnection.h"

#include <dimcore/Dim.h>
#include <wayland-input-method-unstable-v1-client-protocol.h>

#include <QDBusConnection>
#include <QDebug>

#include "utils.h"

using namespace org::deepin::dim;

WLFrontend::WLFrontend()
{
    std::string waylandDisplay = getenv("WAYLAND_DISPLAY");
    if (waylandDisplay.empty()) {
        qDebug("WAYLAND_DISPLAY is not set");
        // todo: fake wayland server
    }

    wl_ = new WaylandConnection(waylandDisplay, this);

    struct wl_registry_listener registry_listener;
    registry_listener.global = CallbackWrapper<&WLFrontend::registryGlobal>::func;
    registry_listener.global_remove = []([[maybe_unused]] void *data,
                                         [[maybe_unused]] struct wl_registry *registry,
                                         uint32_t name) {
        qWarning() << "global_remove" << name;
    };

    auto *registry = wl_display_get_registry(wl_->display());
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_->roundTrip();
    wl_display_flush(wl_->display());

    // zwp_input_method_v1_listener listener;
    // listener.activate = []([[maybe_unused]] void *data,
    //                        [[maybe_unused]] struct zwp_input_method_v1 *zwp_input_method_v1,
    //                        [[maybe_unused]] struct zwp_input_method_context_v1 *id) {
    //     qDebug() << "activate";
    // };
    // listener.deactivate = []([[maybe_unused]] void *data,
    //                          [[maybe_unused]] struct zwp_input_method_v1 *zwp_input_method_v1,
    //                          [[maybe_unused]] struct zwp_input_method_context_v1 *id) {
    //     qDebug() << "deactivate";
    // };
    // zwp_input_method_v1_add_listener(nullptr, &listener, nullptr);
}

WLFrontend::~WLFrontend() { }

void WLFrontend::registryGlobal(struct wl_registry *registry,
                                uint32_t name,
                                const char *interface,
                                uint32_t version)
{
    qWarning() << "global" << name << interface << version;

#define BIND(interface_type)                                                     \
  if (strcmp(interface, #interface_type) == 0) {                                 \
    [[maybe_unused]] auto _ = static_cast<interface_type *>(                     \
        wl_registry_bind(registry, name, &interface_type##_interface, version)); \
  }

    BIND(zwp_input_method_v1);
}

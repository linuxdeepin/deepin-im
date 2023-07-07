#include "WLFrontend.h"

#include "WaylandConnection.h"
#include "WlType.h"
#include "utils.h"

#include <dimcore/Dim.h>
#include <wayland-client-core.h>
#include <wayland-input-method-unstable-v2-client-protocol.h>

#include <QDBusConnection>
#include <QDebug>

using namespace org::deepin::dim;

static const wl_registry_listener registry_listener = {
    CallbackWrapper<&WLFrontend::registryGlobal>::func,
    []([[maybe_unused]] void *data, [[maybe_unused]] struct wl_registry *registry, uint32_t name) {
        qWarning() << "global_remove" << name;
    },
};

WLFrontend::WLFrontend()
{
    std::string waylandDisplay = getenv("WAYLAND_DISPLAY");
    if (waylandDisplay.empty()) {
        qDebug("WAYLAND_DISPLAY is not set");
        // todo: fake wayland server
    }

    wl_ = new WaylandConnection(waylandDisplay, this);

    auto *registry = wl_display_get_registry(wl_->display());
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_->roundTrip();
    wl_display_flush(wl_->display());

    reloadSeats();
}

WLFrontend::~WLFrontend() { }

void WLFrontend::registryGlobal([[maybe_unused]] struct wl_registry *registry,
                                uint32_t name,
                                const char *interface,
                                uint32_t version)
{
    qWarning() << "global" << name << interface << version;

#define BIND(interface_type)                                                         \
  if (strcmp(interface, #interface_type) == 0) {                                     \
    auto p = std::make_shared<WlType<interface_type>>(static_cast<interface_type *>( \
        wl_registry_bind(registry, name, &interface_type##_interface, version)));    \
    globals_[WlType<interface_type>::key()].emplace(std::make_pair(name, p));        \
    return;                                                                          \
  }

    BIND(wl_seat);
    BIND(zwp_input_method_manager_v2);
    BIND(zwp_input_method_v2);
}

void WLFrontend::reloadSeats()
{
    zwp_input_method_manager_v2 *imManager = getGlobal<zwp_input_method_manager_v2>()->get();
    auto seats = getGlobals<wl_seat>();
    for (auto &it : seats) {
        wl_seat *seat = it->get();
        zwp_input_method_v2 *im = zwp_input_method_manager_v2_get_input_method(imManager, seat);
        qWarning() << "im:" << im;
    }
}

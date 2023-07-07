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

const wl_registry_listener WLFrontend::registry_listener_ = {
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
    wl_registry_add_listener(registry, &registry_listener_, this);
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
    BIND(zwp_virtual_keyboard_manager_v1);
}

void WLFrontend::init()
{
    reloadSeats();
}

void WLFrontend::reloadSeats()
{
    zwp_input_method_manager_v2 *imManager = getGlobal<zwp_input_method_manager_v2>()->get();
    zwp_virtual_keyboard_manager_v1 *vkManager =
        getGlobal<zwp_virtual_keyboard_manager_v1>()->get();

    auto seats = getGlobals<wl_seat>();
    for (auto &it : seats) {
        wl_seat *seat = it->get();
        auto vk = std::make_shared<WlType<zwp_virtual_keyboard_v1>>(
            static_cast<zwp_virtual_keyboard_v1 *>(
                zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(vkManager, seat)));
        auto im = std::make_shared<WlType<zwp_input_method_v2>>(static_cast<zwp_input_method_v2 *>(
            zwp_input_method_manager_v2_get_input_method(imManager, seat)));

        ims_.emplace_back(std::make_shared<WaylandInputContextV2>(im, vk));
    }

    wl_->flush();
}

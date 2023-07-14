#include "WLFrontend.h"

#include "utils.h"
#include "wl/Connection.h"
#include "wl/Type.h"

#include <dimcore/Dim.h>
#include <wayland-client-core.h>
#include <wayland-input-method-unstable-v2-client-protocol.h>

#include <QDBusConnection>
#include <QDebug>

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(WLFrontend)

const wl_registry_listener WLFrontend::registry_listener_ = {
    CallbackWrapper<&WLFrontend::global>::func,
    CallbackWrapper<&WLFrontend::globalRemove>::func,
};

WLFrontend::WLFrontend(Dim *dim)
    : FrontendAddon(dim, "wlfrontend")
{
    std::string waylandDisplay = getenv("WAYLAND_DISPLAY");
    if (waylandDisplay.empty()) {
        qDebug("WAYLAND_DISPLAY is not set");
        // todo: fake wayland server
    }

    wl_ = new wl::Connection(waylandDisplay, this);

    auto *registry = wl_display_get_registry(wl_->display());
    wl_registry_add_listener(registry, &registry_listener_, this);
    wl_->roundtrip();
    wl_display_flush(wl_->display());

    reloadSeats();
}

WLFrontend::~WLFrontend() { }

void WLFrontend::global([[maybe_unused]] struct wl_registry *registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version)
{
    qWarning() << "global" << name << interface << version;

#define BIND(interface_type)                                                           \
  if (strcmp(interface, #interface_type) == 0) {                                       \
    auto p = std::make_shared<wl::Type<interface_type>>(static_cast<interface_type *>( \
        wl_registry_bind(registry, name, &interface_type##_interface, version)));      \
    globals_[wl::Type<interface_type>::key()].emplace(std::make_pair(name, p));        \
    return;                                                                            \
  }

    BIND(wl_seat);
    BIND(zwp_input_method_manager_v2);
    BIND(zwp_virtual_keyboard_manager_v1);
}

void WLFrontend::globalRemove([[maybe_unused]] struct wl_registry *wl_registry, uint32_t name)
{
    qWarning() << "global_remove" << name;
    auto iter = globals_.find(name);
    if (iter == globals_.end()) {
        return;
    }

    globals_.erase(iter);
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
    for (auto &seat : seats) {
        auto vk = std::make_shared<wl::Type<zwp_virtual_keyboard_v1>>(
            static_cast<zwp_virtual_keyboard_v1 *>(
                zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(vkManager, seat->get())));
        auto im =
            std::make_shared<wl::Type<zwp_input_method_v2>>(static_cast<zwp_input_method_v2 *>(
                zwp_input_method_manager_v2_get_input_method(imManager, seat->get())));

        ims_.emplace_back(std::make_shared<WaylandInputContextV2>(dim(), im, vk));
    }

    wl_->flush();
}

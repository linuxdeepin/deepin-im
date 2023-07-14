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

WLFrontend::WLFrontend(Dim *dim)
    : FrontendAddon(dim, "wlfrontend")
{
    std::string waylandDisplay = getenv("WAYLAND_DISPLAY");
    if (waylandDisplay.empty()) {
        qDebug("WAYLAND_DISPLAY is not set");
        // todo: fake wayland server
    }

    wl_ = new wl::Connection(waylandDisplay, this);

    reloadSeats();
}

WLFrontend::~WLFrontend() { }

void WLFrontend::init()
{
    reloadSeats();
}

void WLFrontend::reloadSeats()
{
    auto *imManager = wl_->getGlobal<zwp_input_method_manager_v2>()->get();
    auto *vkManager = wl_->getGlobal<zwp_virtual_keyboard_manager_v1>()->get();
    auto seats = wl_->getGlobals<wl_seat>();

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

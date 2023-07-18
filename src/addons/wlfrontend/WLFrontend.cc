#include "WLFrontend.h"

#include "wl/client/Connection.h"
#include "wl/client/Seat.h"
#include "wl/client/ZwpInputMethodManagerV2.h"
#include "wl/client/ZwpVirtualKeyboardManagerV1.h"

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
    auto imManager = wl_->getGlobal<wl::ZwpInputMethodManagerV2>();
    auto vkManager = wl_->getGlobal<wl::ZwpVirtualKeyboardManagerV1>();
    auto seats = wl_->getGlobals<wl::Seat>();

    for (auto &seat : seats) {
        auto vk = vkManager->createVirtualKeyboard(seat);
        auto im = imManager->getInputMethod(seat);

        ims_.emplace_back(std::make_shared<WaylandInputContextV2>(dim(), im, vk));
    }

    wl_->flush();
}

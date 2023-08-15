// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    QByteArray waylandDisplay = qgetenv("WAYLAND_DISPLAY");
    if (waylandDisplay.toStdString().empty()) {
        qDebug("WAYLAND_DISPLAY is not set");
        // todo: fake wayland server
    }

    wl_ = new wl::client::Connection(waylandDisplay.toStdString().c_str(), this);

    reloadSeats();
}

WLFrontend::~WLFrontend() { }

void WLFrontend::init()
{
    reloadSeats();
}

void WLFrontend::reloadSeats()
{
    auto seats = wl_->getGlobals<wl::client::Seat>();
    auto imManager = wl_->getGlobal<wl::client::ZwpInputMethodManagerV2>();
    auto vkManager = wl_->getGlobal<wl::client::ZwpVirtualKeyboardManagerV1>();

    for (auto &seat : seats) {
        auto vk = vkManager->createVirtualKeyboard(seat);
        auto im = imManager->getInputMethod(seat);

        ims_.emplace_back(std::make_shared<WaylandInputContextV2>(dim(), im, vk));
    }

    wl_->flush();
}

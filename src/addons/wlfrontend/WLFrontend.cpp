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
#include <QSocketNotifier>

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

    wl_.reset(new wl::client::Connection(waylandDisplay.toStdString().c_str()));
    auto *notifier = new QSocketNotifier(wl_->getFd(), QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, [this]() {
        wl_->dispatch();
    });

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
        auto vk = vkManager->create_virtual_keyboard(seat);
        auto im = imManager->get_input_method(seat);

        ims_.emplace_back(std::make_shared<DimTextInputV1>(dim(), im, vk));
    }

    wl_->flush();
}

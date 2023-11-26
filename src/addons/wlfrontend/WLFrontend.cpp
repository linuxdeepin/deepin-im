// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WLFrontend.h"

#include "InputMethodV2.h"
#include "WaylandInputContext.h"
#include "wl/client/Compositor.h"
#include "wl/client/Connection.h"
#include "wl/client/ConnectionRaw.h"
#include "wl/client/Seat.h"
#include "wl/client/Surface.h"
#include "wl/client/ZwpInputMethodManagerV2.h"
#include "wl/client/ZwpVirtualKeyboardManagerV1.h"
#include "wl/client/ZwpVirtualKeyboardV1.h"

#include <dimcore/Dim.h>
#include <qpa/qplatformnativeinterface.h>
#include <wayland-client-core.h>
#include <wayland-input-method-unstable-v2-client-protocol.h>

#include <QAbstractEventDispatcher>
#include <QDBusConnection>
#include <QDebug>
#include <QSocketNotifier>
#include <QThread>
#include <QtGui/QGuiApplication>

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(WLFrontend)

WLFrontend::WLFrontend(Dim *dim)
    : FrontendAddon(dim, "wlfrontend")
{
    auto *display = wl_display_connect(nullptr);

    auto *wl = new wl::client::ConnectionRaw(display);
    if (wl->display() == nullptr) {
        return;
    }

    wl_.reset(wl);

    compositor_ = wl_->getGlobal<wl::client::Compositor>();
    surface_ = std::make_shared<wl::client::Surface>(compositor_->create_surface());

    reloadSeats();
}

WLFrontend::~WLFrontend() { }

void WLFrontend::reloadSeats()
{
    const auto seats = wl_->getGlobals<wl::client::Seat>();
    const auto imManager = wl_->getGlobal<wl::client::ZwpInputMethodManagerV2>();
    const auto vkManager = wl_->getGlobal<wl::client::ZwpVirtualKeyboardManagerV1>();

    for (const auto &seat : seats) {
        auto vk = std::make_shared<wl::client::ZwpVirtualKeyboardV1>(
            vkManager->create_virtual_keyboard(seat));
        auto im = std::make_shared<InputMethodV2>(imManager->get_input_method(seat), dim());
        auto wic = std::make_unique<WaylandInputContext>(im, vk, surface_, dim());

        ims_.emplace(seat, std::move(wic));
    }

    wl_->flush();
}

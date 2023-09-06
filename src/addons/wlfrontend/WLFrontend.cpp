// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WLFrontend.h"

#include "wl/client/Connection.h"
#include "wl/client/ConnectionRaw.h"
#include "wl/client/Seat.h"
#include "wl/client/ZwpInputMethodManagerV2.h"
#include "wl/client/ZwpVirtualKeyboardManagerV1.h"

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
    if (QGuiApplication::platformName() == "wayland") {
        QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
        struct wl_display *wl_dpy =
            (struct wl_display *)native->nativeResourceForWindow("display", NULL);

        wl_.reset(new wl::client::ConnectionRaw(wl_dpy));
    } else {
        QByteArray waylandDisplay = qgetenv("DIM_WAYLAND_DISPLAY");
        const auto displayName = waylandDisplay.toStdString();
        if (displayName.empty()) {
            qWarning("failed to get display env");
            return;
        }

        auto *wl = new wl::client::Connection(displayName);
        if (wl->display() == nullptr) {
            return;
        }
        auto *notifier = new QSocketNotifier(wl->getFd(), QSocketNotifier::Read, this);
        connect(notifier, &QSocketNotifier::activated, this, [wl]() {
            wl->dispatch();
        });

        wl_.reset(wl);
        QAbstractEventDispatcher *dispatcher = QThread::currentThread()->eventDispatcher();
        QObject::connect(dispatcher, &QAbstractEventDispatcher::aboutToBlock, this, [this]() {
            wl_->flush();
        });
    }

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

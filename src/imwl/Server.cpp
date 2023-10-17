// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Server.h"

#include "Compositor.h"
#include "DimTextInputManagerV1.h"
#include "DimTextInputV1.h"
#include "InputMethodManagerV2.h"
#include "Seat.h"
#include "VirtualKeyboardManagerV1.h"
#include "X11ActiveWindowMonitor.h"

#include <QAbstractEventDispatcher>
#include <QDebug>
#include <QSocketNotifier>
#include <QThread>

Server::Server()
    : activeWindowMonitor_(std::make_unique<X11ActiveWindowMonitor>())
{
    auto *loop = wl_display_get_event_loop(display());
    int fd = wl_event_loop_get_fd(loop);

    auto processWaylandEvents = [this, loop] {
        int ret = wl_event_loop_dispatch(loop, 0);
        if (ret)
            qWarning() << "wl_event_loop_dispatch error:" << ret;
        wl_display_flush_clients(display());
    };

    noti_.reset(new QSocketNotifier(fd, QSocketNotifier::Read));
    QObject::connect(noti_.get(), &QSocketNotifier::activated, processWaylandEvents);

    QAbstractEventDispatcher *dispatcher = QThread::currentThread()->eventDispatcher();
    QObject::connect(dispatcher, &QAbstractEventDispatcher::aboutToBlock, processWaylandEvents);

    QObject::connect(activeWindowMonitor_.get(),
                     &X11ActiveWindowMonitor::activeWindowChanged,
                     [this](xcb_window_t window) {
                         activeWindowChanged(activeWindowMonitor_->windowPid(window));
                     });
}

Server::~Server() { }

void Server::create()
{
    compositor_ = std::make_unique<Compositor>();
    compositor_->init(display());

    seat_ = std::make_unique<Seat>();
    seat_->init(display());

    dimTextInputManagerV1_ = std::make_unique<DimTextInputManagerV1>();
    dimTextInputManagerV1_->init(display());

    // textInputManagerV3_ = std::make_unique<TextInputManagerV3>();
    // textInputManagerV3_->init(display());

    inputMethodManagerV2_ = std::make_unique<InputMethodManagerV2>();
    inputMethodManagerV2_->init(display());

    virtualKeyboardManagerV1_ = std::make_unique<VirtualKeyboardManagerV1>();
    virtualKeyboardManagerV1_->init(display());

    activeWindowChanged(activeWindowMonitor_->windowPid(activeWindowMonitor_->activeWindow()));
}

void Server::activeWindowChanged(pid_t pid)
{
    if (activePid_ == pid) {
        return;
    }

    qDebug() << "active window changed, pid:" << pid;
    seat_->getDimTextInputV1()->leavePid(activePid_);
    seat_->getDimTextInputV1()->enterPid(pid);
    activePid_ = pid;
}

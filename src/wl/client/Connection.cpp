// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Connection.h"

#include "common/common.h"

#include <QDebug>

#include <poll.h>

using namespace wl::client;

Connection::Connection(const std::string &name, QObject *parent)
    : ConnectionBase(parent)
    , display_(wl_display_connect(name.empty() ? nullptr : name.c_str()))
{
    if (!display_) {
        qWarning() << "Failed to connect to Wayland server" << wl_display_get_error(display());
    }

    init();
}

Connection::~Connection() { }

void Connection::init()
{
    ConnectionBase::init();

    qDebug() << "wl_display_prepare_read";
    while (wl_display_prepare_read(display()) < 0) {
        wl_display_dispatch_pending(display());
    }
    wl_display_flush(display());

    fd_ = wl_display_get_fd(display());
    if (fd_ < 0) {
        qWarning() << "Failed to get Wayland display fd";
        return;
    }

    notifier_ = new QSocketNotifier(fd_, QSocketNotifier::Read, this);
    connect(notifier_, &QSocketNotifier::activated, this, &Connection::dispatch);
}

void Connection::dispatch()
{
    if (display_ == nullptr) {
        return;
    }

    qDebug() << "dispatch";

    if (wl_display_read_events(display()) < 0) {
        qWarning() << "failed to read events from the Wayland socket";
        return;
    }

    while (wl_display_prepare_read(display()) != 0) {
        if (wl_display_dispatch_pending(display()) < 0) {
            qWarning() << "failed to dispatch pending Wayland events";
            return;
        }
    }

    flush();
}

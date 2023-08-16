// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Connection.h"

#include "common/common.h"

#include <gtk/gtk.h>

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

static bool dispatch(GIOChannel *channel, int cond, void *data)
{
    Q_UNUSED(channel);
    Q_UNUSED(cond);

    qDebug() << "dispatch";

    auto self = (Connection *)data;

    if (self->display() == nullptr) {
        return false;
    }

    if (wl_display_read_events(self->display()) < 0) {
        qWarning() << "failed to read events from the Wayland socket";
        return false;
    }

    while (wl_display_prepare_read(self->display()) != 0) {
        if (wl_display_dispatch_pending(self->display()) < 0) {
            qWarning() << "failed to dispatch pending Wayland events";
            return false;
        }
    }

    self->flush();
    return true;
}

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

    GIOChannel *channel = g_io_channel_unix_new(fd_);
    g_io_add_watch(channel, (GIOCondition)(G_IO_IN), (GIOFunc)dispatch, this);
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "XCB.h"

#include <QDebug>
#include <QSocketNotifier>

XCB::XCB()
    : QObject()
{
    xconn_.reset(xcb_connect(nullptr, &defaultScreenNbr_));

    if (int err = xcb_connection_has_error(xconn_.get())) {
        qWarning() << "xcb connect failed";
        return;
    }

    xcbFd_ = xcb_get_file_descriptor(xconn_.get());
    socketNotifier_ = new QSocketNotifier(xcbFd_, QSocketNotifier::Type::Read, this);
    connect(socketNotifier_, &QSocketNotifier::activated, this, &XCB::onXCBEvent);
}

XCB::~XCB() { }

void XCB::onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent)
{
    std::unique_ptr<xcb_generic_event_t> event;
    while (event.reset(xcb_poll_for_event(xconn_.get())), event) {
        xcbEvent(event);
    }
}

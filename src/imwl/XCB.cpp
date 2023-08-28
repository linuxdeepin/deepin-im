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

    setup_ = xcb_get_setup(xconn_.get());
    screen_ = screenOfDisplay(defaultScreenNbr_);

    xcbFd_ = xcb_get_file_descriptor(xconn_.get());
    socketNotifier_ = new QSocketNotifier(xcbFd_, QSocketNotifier::Type::Read, this);
    connect(socketNotifier_, &QSocketNotifier::activated, this, &XCB::onXCBEvent);
}

XCB::~XCB() { }

xcb_screen_t *XCB::screenOfDisplay(int screen)
{
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup_);
    for (; iter.rem; --screen, xcb_screen_next(&iter)) {
        if (screen == 0) {
            return iter.data;
        }
    }

    return nullptr;
}
void XCB::onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent)
{
    std::unique_ptr<xcb_generic_event_t> event;
    while (event.reset(xcb_poll_for_event(xconn_.get())), event) {
        xcbEvent(event);
    }
}

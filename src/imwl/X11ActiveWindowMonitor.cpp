// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "X11ActiveWindowMonitor.h"

#include <xcb/xproto.h>

#include <QDebug>

X11ActiveWindowMonitor::X11ActiveWindowMonitor()
    : XCB()
{
    activeWindowAtom_ = getAtom("_NET_ACTIVE_WINDOW");
    wmPidAtom_ = getAtom("_NET_WM_PID");

    uint32_t values[] = { XCB_EVENT_MASK_PROPERTY_CHANGE };
    xcb_change_window_attributes(xconn_.get(), screen()->root, XCB_CW_EVENT_MASK, values);

    xcb_flush(xconn_.get());
}

X11ActiveWindowMonitor::~X11ActiveWindowMonitor() { }

void X11ActiveWindowMonitor::xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event)
{
    auto responseType = XCB_EVENT_RESPONSE_TYPE(event);
    if (responseType != XCB_PROPERTY_NOTIFY) {
        return;
    }

    auto *e = reinterpret_cast<xcb_property_notify_event_t *>(event.get());
    if (e->atom != activeWindowAtom_) {
        return;
    }

    auto data = getProperty(screen()->root, activeWindowAtom_, sizeof(xcb_window_t));
    if (data.size() == 0) {
        qWarning() << "failed to get active window id";
        return;
    }
    xcb_window_t windowId = *reinterpret_cast<xcb_window_t *>(data.data());

    auto data1 = getProperty(windowId, wmPidAtom_, sizeof(uint32_t));
    if (data1.size() == 0) {
        qWarning() << "failed to get pid of active window";
        return;
    }

    uint32_t pid = *reinterpret_cast<uint32_t *>(data1.data());

    emit activeWindowChanged(pid);
}

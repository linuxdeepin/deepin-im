// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "X11ActiveWindowMonitor.h"

#include <xcb/xproto.h>

#include <QDebug>

X11ActiveWindowMonitor::X11ActiveWindowMonitor()
    : Xcb()
    , activeWindow_("_NET_ACTIVE_WINDOW")
    , wmPid_("_NET_WM_PID")
{
    uint32_t values[] = { XCB_EVENT_MASK_PROPERTY_CHANGE };
    xcb_change_window_attributes(xconn_.get(), screen()->root, XCB_CW_EVENT_MASK, values);

    xcb_flush(xconn_.get());
}

X11ActiveWindowMonitor::~X11ActiveWindowMonitor() { }

pid_t X11ActiveWindowMonitor::activeWindowPid()
{
    auto data = getProperty(screen()->root, activeWindow_, sizeof(xcb_window_t));
    if (data.size() == 0) {
        qWarning() << "failed to get active window id";
        return 0;
    }
    xcb_window_t windowId = *reinterpret_cast<xcb_window_t *>(data.data());

    auto data1 = getProperty(windowId, wmPid_, sizeof(uint32_t));
    if (data1.size() == 0) {
        qWarning() << "failed to get pid of active window";
        return 0;
    }

    uint32_t pid = *reinterpret_cast<uint32_t *>(data1.data());
    return pid;
}

std::tuple<uint16_t, uint16_t> X11ActiveWindowMonitor::activeWindowPosition()
{
    auto data = getProperty(screen()->root, activeWindow_, sizeof(xcb_window_t));
    if (data.size() == 0) {
        qWarning() << "failed to get active window id";
        return {};
    }
    xcb_window_t windowId = *reinterpret_cast<xcb_window_t *>(data.data());

    auto geom = XCB_REPLY(xcb_get_geometry, xconn_.get(), windowId);
    auto offset = XCB_REPLY(xcb_translate_coordinates,
                            xconn_.get(),
                            windowId,
                            screen()->root,
                            geom->x,
                            geom->y);
    return { offset->dst_x, offset->dst_y };
}

void X11ActiveWindowMonitor::xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event)
{
    auto responseType = XCB_EVENT_RESPONSE_TYPE(event);
    if (responseType != XCB_PROPERTY_NOTIFY) {
        return;
    }

    auto *e = reinterpret_cast<xcb_property_notify_event_t *>(event.get());
    if (e->atom != getAtom(activeWindow_)) {
        return;
    }

    emit activeWindowChanged();
}

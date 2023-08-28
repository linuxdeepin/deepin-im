#include "X11ActiveWindowMonitor.h"

#include <QDebug>

X11ActiveWindowMonitor::X11ActiveWindowMonitor()
    : XCB()
{
    const char *activeWindowAtomName = "_NET_ACTIVE_WINDOW";
    auto reply = XCB_REPLY(xcb_intern_atom,
                           xconn_.get(),
                           0,
                           strlen(activeWindowAtomName),
                           activeWindowAtomName);
    if (!reply) {
        throw std::runtime_error("xcb_intern_atom failed");
    }

    activeWindowAtom_ = reply->atom;
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

    qWarning() << "active window================";
}

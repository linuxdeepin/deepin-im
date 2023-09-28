#include "X11AppMonitor.h"

#include <QDebug>

X11AppMonitor::X11AppMonitor()
{
    atomActiveWindow_ = getAtom("_NET_ACTIVE_WINDOW");
    atomNetClientList_ = getAtom("_NET_CLIENT_LIST");
    atomWmPid_ = getAtom("_NET_WM_PID");

    uint32_t values[] = { XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
                          | XCB_EVENT_MASK_PROPERTY_CHANGE };
    xcb_change_window_attributes(xconn_.get(), screen()->root, XCB_CW_EVENT_MASK, values);

    xcb_flush(xconn_.get());

    init();
}

X11AppMonitor::~X11AppMonitor() = default;

void X11AppMonitor::xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event)
{
    auto responseType = XCB_EVENT_RESPONSE_TYPE(event);

    switch (responseType) {
    case XCB_CREATE_NOTIFY: {
        auto *e = reinterpret_cast<xcb_create_notify_event_t *>(event.get());
        pid_t pid = getWindowPid(e->window);
        apps_.emplace(windowToString(e->window), pid);
    } break;
    case XCB_DESTROY_NOTIFY: {
        auto *e = reinterpret_cast<xcb_destroy_notify_event_t *>(event.get());
        apps_.erase(windowToString(e->window));
    } break;
    case XCB_CONFIGURE_NOTIFY: {
        auto *e = reinterpret_cast<xcb_configure_notify_event_t *>(event.get());
    } break;

    case XCB_PROPERTY_NOTIFY: {
        auto *e = reinterpret_cast<xcb_property_notify_event_t *>(event.get());
        if (e->atom != atomActiveWindow_) {
            return;
        }

        auto data = getProperty(screen()->root, atomActiveWindow_, sizeof(xcb_window_t));
        if (data.size() == 0) {
            qWarning() << "failed to get active window id";
            return;
        }

        xcb_window_t window = *reinterpret_cast<xcb_window_t *>(data.data());
        auto windowTmp = windowToString(window);
        if (focus_ == windowTmp) {
            return;
        }

        qWarning() << window << windowTmp << stringToWindow(windowTmp);
        focus_ = windowTmp;
    } break;
    default:
        return;
    }

    emit appUpdated(apps_, focus_);
}

void X11AppMonitor::init()
{
    auto data = getProperty(screen()->root, atomNetClientList_);
    for (int i = 0; i < data.size(); i += sizeof(xcb_window_t)) {
        xcb_window_t window = *reinterpret_cast<xcb_window_t *>(data.data() + i);
        pid_t pid = getWindowPid(window);

        apps_.emplace(windowToString(window), pid);
    }
}

pid_t X11AppMonitor::getWindowPid(xcb_window_t window)
{
    auto data1 = getProperty(window, atomWmPid_, sizeof(uint32_t));
    if (data1.size() == 0) {
        qWarning() << "failed to get pid of active window";
        return 0;
    }

    return *reinterpret_cast<uint32_t *>(data1.data());
}

std::tuple<uint16_t, uint16_t> X11AppMonitor::getWindowPosition(xcb_window_t window)
{
    auto geom = XCB_REPLY(xcb_get_geometry, xconn_.get(), window);
    auto offset = XCB_REPLY(xcb_translate_coordinates,
                            xconn_.get(),
                            window,
                            screen()->root,
                            geom->x,
                            geom->y);
    return { offset->dst_x, offset->dst_y };
}

QString X11AppMonitor::windowToString(xcb_window_t window)
{
    if (window == 0) {
        return QString{};
    }

    return QString("0x%1").arg(window, 8, 16, QLatin1Char('0'));
}

xcb_window_t X11AppMonitor::stringToWindow(const QString &string)
{
    bool ok = false;
    return string.toUInt(&ok, 16);
}

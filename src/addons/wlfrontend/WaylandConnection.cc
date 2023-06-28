#include "WaylandConnection.h"

#include <wayland-client-core.h>

#include <QDebug>

using namespace org::deepin::dim;

WaylandConnection::WaylandConnection(const std::string &name, QObject *parent)
    : QObject(parent)
    , display_(wl_display_connect(name.empty() ? nullptr : name.c_str()))
{
    if (!display_) {
        qWarning() << "Failed to connect to Wayland server" << wl_display_get_error(display_);
    }

    init();
}

WaylandConnection::~WaylandConnection()
{
    if (display_) {
        wl_display_disconnect(display_);
        display_ = nullptr;
    }
}

void WaylandConnection::init()
{
    int fd = wl_display_get_fd(display_);
    if (fd < 0) {
        qWarning() << "Failed to get Wayland display fd";
        return;
    }

    while (wl_display_prepare_read(display_) != 0) {
        wl_display_dispatch_pending(display_);
    }
    wl_display_flush(display_);

    notifier_ = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(notifier_, &QSocketNotifier::activated, this, &WaylandConnection::dispatch);
}

void WaylandConnection::dispatch()
{
    wl_display_read_events(display_);
    wl_display_dispatch_pending(display_);
}

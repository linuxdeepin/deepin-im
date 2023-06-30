#include "WaylandConnection.h"

#include <wayland-client-core.h>

#include <QDebug>

#include <poll.h>

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
    fd_ = wl_display_get_fd(display_);
    if (fd_ < 0) {
        qWarning() << "Failed to get Wayland display fd";
        return;
    }

    notifier_ = new QSocketNotifier(fd_, QSocketNotifier::Read, this);
    connect(notifier_, &QSocketNotifier::activated, this, &WaylandConnection::dispatch);
}

void WaylandConnection::dispatch()
{
    qWarning() << "dispatch";
    if (display_ == nullptr) {
        return;
    }

    struct pollfd pfd;
    pfd.fd = fd_;
    pfd.events = POLLIN;
    int ret = poll(&pfd, 1, 0);
    if (ret > 0) {
        wl_display_read_events(display_);
    } else {
        wl_display_cancel_read(display_);
    }

    if (wl_display_dispatch_pending(display_) == -1) {
        int error = wl_display_get_error(display_);
        if (error != 0) {
            free(display_);
            display_ = nullptr;
            return;
        }
    }
}

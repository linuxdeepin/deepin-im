#include "Connection.h"

#include <wayland-client-core.h>

#include <QDebug>

#include <poll.h>

using namespace wl;

Connection::Connection(const std::string &name, QObject *parent)
    : QObject(parent)
    , display_(wl_display_connect(name.empty() ? nullptr : name.c_str()))
{
    if (!display_) {
        qWarning() << "Failed to connect to Wayland server" << wl_display_get_error(display_.get());
    }

    init();
}

Connection::~Connection()
{
    if (display_) {
        wl_display_disconnect(display_.get());
        display_ = nullptr;
    }
}

void Connection::init()
{
    fd_ = wl_display_get_fd(display_.get());
    if (fd_ < 0) {
        qWarning() << "Failed to get Wayland display fd";
        return;
    }

    notifier_ = new QSocketNotifier(fd_, QSocketNotifier::Read, this);
    connect(notifier_, &QSocketNotifier::activated, this, &Connection::dispatch);
}

void Connection::dispatch()
{
    qWarning() << "dispatch";
    if (display_ == nullptr) {
        return;
    }

    roundtrip();
}

void Connection::roundtrip()
{
    wl_display_roundtrip(display_.get());
}

void Connection::flush()
{
    wl_display_flush(display_.get());
}

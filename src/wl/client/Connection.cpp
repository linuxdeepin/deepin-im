#include "Connection.h"

#include "common/common.h"

#include <QDebug>

#include <poll.h>

using namespace wl::client;

const wl_registry_listener Connection::registry_listener_ = {
    CallbackWrapper<&Connection::onGlobal>::func,
    CallbackWrapper<&Connection::onGlobalRemove>::func,
};

Connection::Connection(const std::string &name, QObject *parent)
    : QObject(parent)
    , display_(wl_display_connect(name.empty() ? nullptr : name.c_str()))
{
    if (!display_) {
        qWarning() << "Failed to connect to Wayland server" << wl_display_get_error(display_.get());
    }

    auto *registry = wl_display_get_registry(display_.get());
    wl_registry_add_listener(registry, &registry_listener_, this);
    roundtrip();

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
    qDebug() << "wl_display_prepare_read";
    while (wl_display_prepare_read(display_.get()) < 0) {
        wl_display_dispatch_pending(display_.get());
    }
    wl_display_flush(display_.get());

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
    if (display_ == nullptr) {
        return;
    }

    qDebug() << "dispatch";

    if (wl_display_read_events(display_.get()) < 0) {
        qWarning() << "failed to read events from the Wayland socket";
        return;
    }

    while (wl_display_prepare_read(display_.get()) != 0) {
        if (wl_display_dispatch_pending(display_.get()) < 0) {
            qWarning() << "failed to dispatch pending Wayland events";
            return;
        }
    }

    wl_display_flush(display_.get());
}

void Connection::roundtrip()
{
    wl_display_roundtrip(display_.get());
}

void Connection::flush()
{
    wl_display_flush(display_.get());
}

void Connection::onGlobal([[maybe_unused]] struct wl_registry *registry,
                          uint32_t name,
                          const char *interface,
                          uint32_t version)
{
    qWarning() << "onGlobal:" << name << interface << version;
    auto &gi = globals_[interface];
    gi.version = version;
    gi.names.emplace(name);
}

void Connection::onGlobalRemove([[maybe_unused]] struct wl_registry *wl_registry, uint32_t name)
{
    for (auto &gi : globals_) {
        gi.second.names.erase(name);
    }
}

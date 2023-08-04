#include "ConnectionBase.h"

#include "common/common.h"

#include <QDebug>

#include <poll.h>

using namespace wl::client;

const wl_registry_listener ConnectionBase::registry_listener_ = {
    CallbackWrapper<&ConnectionBase::onGlobal>::func,
    CallbackWrapper<&ConnectionBase::onGlobalRemove>::func,
};

ConnectionBase::ConnectionBase(QObject *parent)
    : QObject(parent)
{
}

ConnectionBase::~ConnectionBase() { }

void ConnectionBase::init()
{
    auto *registry = wl_display_get_registry(display());
    wl_registry_add_listener(registry, &registry_listener_, this);
    roundtrip();
}

void ConnectionBase::dispatch()
{
    qDebug() << "dispatch";

    qWarning() << "1111111111111111111111111";
    if (wl_display_read_events(display()) < 0) {
        qWarning() << "failed to read events from the Wayland socket";
        return;
    }

    qWarning() << "2222222222222222222222222";
    while (wl_display_prepare_read(display()) != 0) {
        qWarning() << "333333333333333333333333333";
        if (wl_display_dispatch_pending(display()) < 0) {
            qWarning() << "failed to dispatch pending Wayland events";
            return;
        }
    }

    qWarning() << "444444444444444444444444444444";
    wl_display_flush(display());
    qWarning() << "5555555555555555555555555555555";
}

void ConnectionBase::roundtrip()
{
    wl_display_roundtrip(display());
}

void ConnectionBase::flush()
{
    wl_display_flush(display());
}

void ConnectionBase::onGlobal([[maybe_unused]] struct wl_registry *registry,
                              uint32_t name,
                              const char *interface,
                              uint32_t version)
{
    qWarning() << "onGlobal:" << name << interface << version;
    auto &gi = globals_[interface];
    gi.version = version;
    gi.names.emplace(name);
}

void ConnectionBase::onGlobalRemove([[maybe_unused]] struct wl_registry *wl_registry, uint32_t name)
{
    for (auto &[_, t] : bindedGlobals_) {
        if (t.erase(name)) {
            break;
        }
    }

    for (auto &gi : globals_) {
        if (gi.second.names.erase(name)) {
            break;
        }
    }
}

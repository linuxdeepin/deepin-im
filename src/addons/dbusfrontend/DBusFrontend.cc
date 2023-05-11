#include "DBusFrontend.h"

#include <QDBusConnection>

#include "PortalAdaptor.h"

DIM_ADDON_FACTORY(DBusFrontend)

DBusFrontend::DBusFrontend(Dim *dim)
    : FrontendAddon(dim)
    , adaptor_(new PortalAdaptor(this)) {
    QDBusConnection::sessionBus().registerService("org.deepin.dim");
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/portal/inputmethod", this);
}

DBusFrontend::~DBusFrontend() {
}

QDBusObjectPath DBusFrontend::CreateInputContext() {
    return QDBusObjectPath("/");
}

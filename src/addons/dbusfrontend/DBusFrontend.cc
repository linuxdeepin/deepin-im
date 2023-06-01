#include "DBusFrontend.h"

#include <QDBusConnection>

#include <dimcore/Dim.h>

#include "InputmethodAdaptor.h"
#include "InputContext1.h"

DIM_ADDON_FACTORY(DBusFrontend)

DBusFrontend::DBusFrontend(Dim *dim)
    : FrontendAddon(dim, "dbusfrontend")
    , adaptor_(new InputmethodAdaptor(this)) {
    QDBusConnection::sessionBus().registerService("org.deepin.dim");
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/portal/inputmethod", this);
}

DBusFrontend::~DBusFrontend() {
}

QDBusObjectPath DBusFrontend::CreateInputContext() {
    auto *icbus = new InputContext1(dim(), this);

    return QDBusObjectPath(icbus->path());
}

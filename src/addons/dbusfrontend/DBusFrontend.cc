#include "DBusFrontend.h"

#include <QDBusConnection>

#include <dimcore/Dim.h>

#include "InputmethodAdaptor.h"
#include "InputContext1.h"

DIM_ADDON_FACTORY(DBusFrontend)

static QString inputcontextPath = "/org/freedesktop/portal/inputmethod/%1";

DBusFrontend::DBusFrontend(Dim *dim)
    : FrontendAddon(dim)
    , adaptor_(new InputmethodAdaptor(this)) {
    QDBusConnection::sessionBus().registerService("org.deepin.dim");
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/portal/inputmethod", this);
}

DBusFrontend::~DBusFrontend() {
}

QDBusObjectPath DBusFrontend::CreateInputContext() {
    auto *icbus = new InputContext1(this);
    auto id = icbus->id();
    inputContextBuses_.insert(id, icbus);
    connect(icbus, &InputContext1::destroyed, this, [this, id]() { inputContextBuses_.remove(id); });

    return QDBusObjectPath(inputcontextPath.arg(id));
}

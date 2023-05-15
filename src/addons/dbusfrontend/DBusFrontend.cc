#include "DBusFrontend.h"

#include <QDBusConnection>

#include <dimcore/Dim.h>

#include "InputmethodAdaptor.h"
#include "InputContextBus.h"

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
    uint32_t id = dim()->newInputContext();

    auto *icbus = new InputContextBus(id, this);
    inputContextBuses_.insert(id, icbus);
    connect(icbus, &InputContextBus::destroyed, this, [this, id]() { inputContextBuses_.remove(id); });

    return QDBusObjectPath(inputcontextPath.arg(id));
}

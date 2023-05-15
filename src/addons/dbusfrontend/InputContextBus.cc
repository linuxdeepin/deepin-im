#include "InputContextBus.h"

#include <QDBusConnection>

#include "InputcontextAdaptor.h"

InputContextBus::InputContextBus(uint32_t id, QObject *parent)
    : QObject(parent)
    , id_(id)
    , adaptor_(new InputcontextAdaptor(this)) {
    QDBusConnection::sessionBus().registerObject(QString("/org/freedesktop/portal/inputcontext/%1").arg(id), this);
}

InputContextBus::~InputContextBus() {
}

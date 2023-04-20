#include "Dim.h"

#include "InputMethodAdaptor.h"

Dim::Dim(QObject *parent)
    : QObject(parent) {
    new InputmethodAdaptor(this);
}

QDBusObjectPath Dim::CreateInputContext() {
    return QDBusObjectPath("");
}

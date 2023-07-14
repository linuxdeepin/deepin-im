#include "BatchEvent.h"

#include <QDBusMetaType>

void registerBatchEventQtDBusTypes()
{
    qRegisterMetaType<BatchEvent>("BatchEvent");
    qRegisterMetaType<QList<BatchEvent>>("QList<BatchEvent>");

    qDBusRegisterMetaType<BatchEvent>();
    qDBusRegisterMetaType<QList<BatchEvent>>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const BatchEvent &event)
{
    argument.beginStructure();
    argument << event.type;
    argument << QDBusVariant(event.data);
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, BatchEvent &event)
{
    argument.beginStructure();
    argument >> event.type;
    argument >> event.data;
    argument.endStructure();

    return argument;
}

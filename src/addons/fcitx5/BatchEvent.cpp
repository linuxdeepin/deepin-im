// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BatchEvent.h"

#include <QDBusMetaType>

void registerBatchEventQtDBusTypes()
{
    qRegisterMetaType<BatchEvent>("BatchEvent");
    qRegisterMetaType<QList<BatchEvent>>("QList<BatchEvent>");
    qRegisterMetaType<PreeditKeyData>("PreeditKeyData");
    qRegisterMetaType<QList<PreeditKeyData>>("QList<PreeditKeyData>");
    qRegisterMetaType<PreeditKey>("PreeditKey");

    qDBusRegisterMetaType<BatchEvent>();
    qDBusRegisterMetaType<QList<BatchEvent>>();
    qDBusRegisterMetaType<PreeditKeyData>();
    qDBusRegisterMetaType<QList<PreeditKeyData>>();
    qDBusRegisterMetaType<PreeditKey>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const BatchEvent &event)
{
    argument.beginStructure();
    argument << event.type << QDBusVariant(event.data);
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, BatchEvent &event)
{
    argument.beginStructure();
    argument >> event.type >> event.data;
    argument.endStructure();

    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const PreeditKeyData &event)
{
    argument.beginStructure();
    argument << event.text << event.format;
    argument.endStructure();

    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const DBusForwardKey &event) {
    argument.beginStructure();
    argument << event.keysym << event.keystate << event.isRelease;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, DBusForwardKey &event) {
    argument.beginStructure();
    argument >> event.keysym >> event.keystate >> event.isRelease;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, PreeditKeyData &event)
{
    argument.beginStructure();
    argument >> event.text >> event.format;
    argument.endStructure();

    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const PreeditKey &event)
{
    argument.beginStructure();
    argument.beginArray(qMetaTypeId<PreeditKeyData>());
    for (const auto &element : event.info) {
        argument << element;
    }
    argument.endArray();
    argument << event.cursor;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, PreeditKey &event)
{
    argument.beginStructure();
    argument.beginArray();
    while (!argument.atEnd()) {
        PreeditKeyData element;
        argument >> element;
        event.info.append(element);
    }
    argument.endArray();
    argument >> event.cursor;
    argument.endStructure();

    return argument;
}

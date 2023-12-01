// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BATCHEVENT_H
#define BATCHEVENT_H

#include <QDBusArgument>
#include <QVariant>

enum { BATCHED_COMMIT_STRING = 0, BATCHED_PREEDIT, BATCHED_FORWARD_KEY };

struct BatchEvent
{
    uint32_t type;
    QVariant data;
};

struct DBusForwardKey
{
    uint32_t keysym;
    uint32_t keystate;
    bool isRelease;
};

struct PreeditKeyData
{
    QString text;
    int32_t format;
};

struct PreeditKey
{
    QList<PreeditKeyData> info;
    int32_t cursor;
};

Q_DECLARE_METATYPE(BatchEvent)
Q_DECLARE_METATYPE(DBusForwardKey)
Q_DECLARE_METATYPE(PreeditKeyData)
Q_DECLARE_METATYPE(PreeditKey)

void registerBatchEventQtDBusTypes();

QDBusArgument &operator<<(QDBusArgument &argument, const BatchEvent &event);
const QDBusArgument &operator>>(const QDBusArgument &argument, BatchEvent &event);

QDBusArgument &operator<<(QDBusArgument &argument, const DBusForwardKey &event);
const QDBusArgument &operator>>(const QDBusArgument &argument, DBusForwardKey &event);

QDBusArgument &operator<<(QDBusArgument &argument, const PreeditKeyData &event);
const QDBusArgument &operator>>(const QDBusArgument &argument, PreeditKeyData &event);
QDBusArgument &operator<<(QDBusArgument &argument, const PreeditKey &event);
const QDBusArgument &operator>>(const QDBusArgument &argument, PreeditKey &event);

#endif // !BATCHEVENT_H

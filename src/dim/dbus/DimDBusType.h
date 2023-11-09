// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIM_DBUS_TYPE_H
#define DIM_DBUS_TYPE_H

#include <QDBusMetaType>
#include <QMap>
#include <QVariantMap>
#include <QList>

struct InputMethodData
{
    QString addon;
    QString name;
};

struct ConfigOption
{
    QString arg1;
    QString arg2;
    QString arg3;
    QVariant arg4;
    QVariantMap arg5;
};

struct Config
{
    QString name;
    QList<ConfigOption> opt;
};

typedef QList<InputMethodData> InputMethodDataList;
typedef QList<Config> InputMethodConfigList;

void registerDimQtDBusTypes();

QDBusArgument &operator<<(QDBusArgument &argument, const InputMethodData &data);
const QDBusArgument &operator>>(const QDBusArgument &argument, InputMethodData &data);
QDBusArgument &operator<<(QDBusArgument &argument, const Config &data);
const QDBusArgument &operator>>(const QDBusArgument &argument, Config &data);
QDBusArgument &operator<<(QDBusArgument &argument, const ConfigOption &data);
const QDBusArgument &operator>>(const QDBusArgument &argument, ConfigOption &data);

Q_DECLARE_METATYPE(InputMethodData)
Q_DECLARE_METATYPE(InputMethodDataList)
Q_DECLARE_METATYPE(Config)
Q_DECLARE_METATYPE(ConfigOption)
Q_DECLARE_METATYPE(InputMethodConfigList)

#endif // DIM_DBUS_TYPE_H

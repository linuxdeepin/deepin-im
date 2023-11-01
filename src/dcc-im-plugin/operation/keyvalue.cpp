// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#include "keyvalue.h"

#include <QDBusMetaType>

using namespace dcc_im_plugin::operation;

static void registerDBusType()
{
    qRegisterMetaType<KeyValue>("KeyValue");
    qRegisterMetaType<KeyValueList>("KeyValueList");
    qDBusRegisterMetaType<KeyValue>();
    qDBusRegisterMetaType<KeyValueList>();
}
Q_CONSTRUCTOR_FUNCTION(registerDBusType)

QString KeyValue::key() const
{
    return m_key;
}

void KeyValue::setKey(const QString &key)
{
    if (m_key == key) {
        return;
    }
    m_key = key;
}

QString KeyValue::value() const
{
    return m_value;
}

void KeyValue::setValue(const QString &value)
{
    if (m_value == value) {
        return;
    }
    m_value = value;
}

QDBusArgument &operator<<(QDBusArgument &argument, const KeyValue &arg)
{
    argument.beginStructure();
    argument << arg.key();
    argument << arg.value();
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, KeyValue &arg)
{
    QString key, value;
    argument.beginStructure();
    argument >> key >> value;
    argument.endStructure();
    arg.setKey(key);
    arg.setValue(value);
    return argument;
}

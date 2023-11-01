// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "configtype.h"

#include <QDBusMetaType>

using namespace dcc_im_plugin::operation;

static void registerDBusType()
{
    qRegisterMetaType<ConfigType>("ConfigType");
    qRegisterMetaType<ConfigTypeList>("ConfigTypeList");
    qDBusRegisterMetaType<ConfigType>();
    qDBusRegisterMetaType<ConfigTypeList>();
}
Q_CONSTRUCTOR_FUNCTION(registerDBusType)

QString ConfigType::name() const
{
    return m_name;
}

void ConfigType::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
    m_name = name;
}

ConfigOptionList ConfigType::options() const
{
    return m_options;
}

void ConfigType::setOptions(const ConfigOptionList &options)
{
    if (m_options == options) {
        return;
    }
    m_options = options;
}

QDBusArgument &operator<<(QDBusArgument &argument, const ConfigType &value)
{
    argument.beginStructure();
    argument << value.name();
    argument << value.options();
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ConfigType &value)
{
    QString name;
    ConfigOptionList options;
    argument.beginStructure();
    argument >> name >> options;
    argument.endStructure();
    value.setName(name);
    value.setOptions(options);
    return argument;
}

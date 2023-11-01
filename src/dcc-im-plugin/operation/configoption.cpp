// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "configoption.h"

#include <qobject.h>

#include <QDBusMetaType>

using namespace dcc_im_plugin::operation;

static void registerDBusType()
{
    qRegisterMetaType<ConfigOption>("ConfigOption");
    qRegisterMetaType<ConfigOptionList>("ConfigOptionList");
    qDBusRegisterMetaType<ConfigOption>();
    qDBusRegisterMetaType<ConfigOptionList>();
}
Q_CONSTRUCTOR_FUNCTION(registerDBusType)

QDBusArgument &operator<<(QDBusArgument &argument,
                          const dcc_im_plugin::operation::ConfigOption &arg)
{
    argument.beginStructure();
    argument << arg.name();
    argument << arg.type();
    argument << arg.description();
    argument << arg.defaultValue();
    argument << arg.properties();
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                dcc_im_plugin::operation::ConfigOption &arg)
{
    QString name, description, type;
    QDBusVariant defaultValue;
    QVariantMap properties;
    argument.beginStructure();
    argument >> name >> type >> description >> defaultValue >> properties;
    argument.endStructure();
    arg.setName(name);
    arg.setType(type);
    arg.setDescription(description);
    arg.setDefaultValue(defaultValue);
    arg.setProperties(properties);
    return argument;
}

ConfigOption::ConfigOption(const ConfigOption &option)
{
    m_name = option.m_name;
    m_type = option.m_type;
    m_description = option.m_description;
    m_defaultValue = option.m_defaultValue;
    m_properties = option.m_properties;
}

ConfigOption::~ConfigOption() { }

QString ConfigOption::name() const
{
    return m_name;
}

void ConfigOption::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }
    m_name = name;
}

QString ConfigOption::type() const
{
    return m_type;
}

void ConfigOption::setType(const QString &type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;
}

QString ConfigOption::description() const
{
    return m_description;
}

void ConfigOption::setDescription(const QString &description)
{
    if (description == m_description) {
        return;
    }
    m_description = description;
}

QDBusVariant ConfigOption::defaultValue() const
{
    return m_defaultValue;
}

void ConfigOption::setDefaultValue(const QDBusVariant &defaultValue)
{
    if (m_defaultValue == defaultValue) {
        return;
    }
    m_defaultValue = defaultValue;
}

QVariantMap ConfigOption::properties() const
{
    return m_properties;
}

void ConfigOption::setProperties(const QVariantMap &properties)
{
    if (m_properties == properties) {
        return;
    }
    m_properties = properties;
}

bool ConfigOption::operator==(const ConfigOption &configOption)
{
    return m_name == configOption.name() && m_type == configOption.type()
        && m_description == configOption.description()
        && m_defaultValue == configOption.defaultValue()
        && m_properties == configOption.properties();
}

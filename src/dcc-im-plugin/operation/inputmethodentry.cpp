// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "inputmethodentry.h"

#include <QDBusMetaType>
using namespace dcc_im_plugin::operation;

static void registerDBusType()
{
    qRegisterMetaType<InputMethodEntry>("InputMethodEntry");
    qRegisterMetaType<InputMethodEntryList>("InputMethodEntryList");
    qDBusRegisterMetaType<InputMethodEntry>();
    qDBusRegisterMetaType<InputMethodEntryList>();
}
Q_CONSTRUCTOR_FUNCTION(registerDBusType)

QString InputMethodEntry::uniqueName() const
{
    return m_uniqueName;
}

void InputMethodEntry::setUniqueName(const QString &uniqueName)
{
    if (m_uniqueName == uniqueName) {
        return;
    }
    m_uniqueName = uniqueName;
}

QString InputMethodEntry::name() const
{
    return m_name;
}

void InputMethodEntry::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
    m_name = name;
}

QString InputMethodEntry::nativeName() const
{
    return m_nativeName;
}

void InputMethodEntry::setNativeName(const QString &nativeName)
{
    if (m_nativeName == nativeName) {
        return;
    }
    m_nativeName = nativeName;
}

QString InputMethodEntry::icon() const
{
    return m_icon;
}

void InputMethodEntry::setIcon(const QString &icon)
{
    if (m_icon == icon) {
        return;
    }
    m_icon = icon;
}

QString InputMethodEntry::label() const
{
    return m_label;
}

void InputMethodEntry::setLabel(const QString &label)
{
    if (m_label == label) {
        return;
    }
    m_label = label;
}

QString InputMethodEntry::languageCode() const
{
    return m_languageCode;
}

void InputMethodEntry::setLanguageCode(const QString &languageCode)
{
    if (m_languageCode == languageCode) {
        return;
    }
    m_languageCode = languageCode;
}

bool InputMethodEntry::configurable() const
{
    return m_configurable;
}

void InputMethodEntry::setConfigurable(const bool &configurable)
{
    if (m_configurable == configurable) {
        return;
    }
    m_configurable = configurable;
}

bool InputMethodEntry::enabled() const
{
    return m_enabled;
}

void InputMethodEntry::setEnabled(const bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }
    m_enabled = enabled;
}

QDBusArgument &operator<<(QDBusArgument &argument, const InputMethodEntry &arg)
{
    argument.beginStructure();
    argument << arg.uniqueName();
    argument << arg.name();
    argument << arg.nativeName();
    argument << arg.icon();
    argument << arg.label();
    argument << arg.languageCode();
    argument << arg.configurable();
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, InputMethodEntry &arg)
{
    QString uniqueName, name, nativeName, icon, label, languageCode;
    bool configurable;
    argument.beginStructure();
    argument >> uniqueName >> name >> nativeName >> icon >> label >> languageCode >> configurable;
    argument.endStructure();
    arg.setUniqueName(uniqueName);
    arg.setName(name);
    arg.setNativeName(nativeName);
    arg.setIcon(icon);
    arg.setLabel(label);
    arg.setLanguageCode(languageCode);
    arg.setConfigurable(configurable);
    return argument;
}

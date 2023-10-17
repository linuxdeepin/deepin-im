// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodEntry.h"

class InputMethodEntryPrivate
{
public:
    QString addon;
    QString uniqueName;
    QString name;
    QString description;
    QString label;
    QString iconName;
};

using namespace org::deepin::dim;

InputMethodEntry::InputMethodEntry(const QString &addon,
                                   const QString &uniqueName,
                                   const QString &name,
                                   const QString &description,
                                   const QString &label,
                                   const QString &iconName)
    : d(new InputMethodEntryPrivate{ addon, uniqueName, name, description, label, iconName })
{
}

InputMethodEntry::~InputMethodEntry() { }

const QString &InputMethodEntry::addonName() const
{
    return d->addon;
}

const QString &InputMethodEntry::uniqueName() const
{
    return d->uniqueName;
}

const QString &InputMethodEntry::name() const
{
    return d->name;
}

const QString &InputMethodEntry::description() const
{
    return d->description;
}

const QString &InputMethodEntry::label() const
{
    return d->label;
}

const QString &InputMethodEntry::iconName() const
{
    return d->iconName;
}

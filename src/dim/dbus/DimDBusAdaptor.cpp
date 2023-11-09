// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DimDBusAdaptor.h"

#include "dbus/DimDBusType.h"

DimDBusAdaptor::DimDBusAdaptor(Dim *parent)
    : QObject(parent)
{
    registerDimQtDBusTypes();
}

DimDBusAdaptor::~DimDBusAdaptor() { }

void DimDBusAdaptor::AddInputMethod(const QString &addon, const QString &name)
{
    parent()->addInputMethod(addon.toStdString(), name.toStdString());

    Q_EMIT InputMethodsChanged();
}

void DimDBusAdaptor::RemoveInputMethod(const QString &addon, const QString &name)
{
    parent()->removeInputMethod(addon.toStdString(), name.toStdString());

    Q_EMIT InputMethodsChanged();
}

bool DimDBusAdaptor::SetCurrentInputMethod(const QString &addon, const QString &name)
{
    auto ret = parent()->requestSwitchIM(addon.toStdString(), name.toStdString());

    if (ret) {
        Q_EMIT CurrentInputMethodChanged(InputMethodData{ addon, name });
    }

    return ret;
}

InputMethodData DimDBusAdaptor::GetCurrentInputMethod()
{
    return InputMethodData{ QString::fromStdString(parent()->getCurrentInputMethod().first),
                            QString::fromStdString(parent()->getCurrentInputMethod().second) };
}

InputMethodDataList DimDBusAdaptor::GetAvailableInputMethods()
{
    InputMethodDataList availableInputMethods = InputMethodDataList{};
    for (const auto &im : parent()->activeInputMethodEntries()) {
        availableInputMethods.append(
            InputMethodData{ QString::fromStdString(im.first), QString::fromStdString(im.second) });
    }

    return availableInputMethods;
}

InputMethodDataList DimDBusAdaptor::GetCurrentInputMethods()
{
    InputMethodDataList currentActiveIMs = InputMethodDataList{};
    for (const auto &im : parent()->getCurrentInputMethods()) {
        currentActiveIMs.append(
            InputMethodData{ QString::fromStdString(im.first), QString::fromStdString(im.second) });
    }

    return currentActiveIMs;
}

// TODO: implement
void DimDBusAdaptor::Configure(const QString &addon, const QString &name) { }

bool DimDBusAdaptor::SetInputMethodShortcut(const QString &keyStroke)
{
    return false;
}

bool DimDBusAdaptor::SetInputMethodOrderShortcut(const QString &keyStroke)
{
    return false;
}

QDBusVariant DimDBusAdaptor::GetConfig(const QString &addon, const QString &name)
{
    return QDBusVariant();
}

void DimDBusAdaptor::SetConfig(const QString &addon,
                               const QString &name,
                               const QDBusVariant &config)
{
}

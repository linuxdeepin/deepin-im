// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <interface/moduleobject.h>
#include <interface/plugininterface.h>

using namespace DCC_NAMESPACE;

class IMConfigPlugin : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.deepin.dde.ControlCenter.Plugin_im" FILE "plugin-im.json")
    Q_INTERFACES(DCC_NAMESPACE::PluginInterface)
public:
    IMConfigPlugin();
    virtual QString name() const override;
    virtual ModuleObject *module() override;
    virtual QString follow() const override;
    virtual QString location() const override;
};

class LabelModule : public ModuleObject
{
    Q_OBJECT
public:
    LabelModule(QObject *parent = nullptr)
        : ModuleObject(parent)
    {
    }

    LabelModule(const QString &name, const QString &displayName = {}, QObject *parent = nullptr)
        : ModuleObject(name, displayName, parent)
    {
    }

    virtual QWidget *page() override;
};

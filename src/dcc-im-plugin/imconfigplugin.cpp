// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "imconfigplugin.h"

#include "window/imwindow.h"

#include <interface/pagemodule.h>

IMConfigPlugin::IMConfigPlugin() { }

QString IMConfigPlugin::name() const
{
    return tr("deepin-imconfigtool-plugin");
}

QString IMConfigPlugin::follow() const
{
    return "keyboard";
}

QString IMConfigPlugin::location() const
{
    return QString("2");
}

ModuleObject *IMConfigPlugin::module()
{
    ModuleObject *module =
        new PageModule(QString("Manage Input Methods"), tr("Input Method"), this);
    LabelModule *labelModule = new LabelModule();
    module->appendChild(labelModule);

    return module;
}

QWidget *LabelModule::page()
{
    dcc_im_plugin::IMWindow *m_imWindow = new dcc_im_plugin::IMWindow();
    return m_imWindow;
}

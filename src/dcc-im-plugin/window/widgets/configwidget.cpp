// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "configwidget.h"

#include "keylistwidget.h"
#include "operation/configoption.h"
#include "operation/imworker.h"
#include "optionwidget.h"
#include "utils/translate.h"
#include "verticalscrollarea.h"

#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>

using namespace dcc_im_plugin::widgets;
using namespace dcc_im_plugin::operation;
using namespace utilFuncs;

dcc_im_plugin::operation::IMWorker *ConfigWidget::m_worker = nullptr;

// TODO: modify constructor if needed
ConfigWidget::ConfigWidget(dcc_im_plugin::operation::IMWorker *worker,
                           /*const QString &uri, DBusProvider *dbus,*/ QWidget *parent)
    : QWidget(parent)
    /* , uri_(uri) */
    /* , dbus_(dbus) */
    , mainWidget_(new QWidget(this))
{
    m_worker = worker;
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mainWidget_);
    setLayout(layout);
    connect(m_worker,
            &IMWorker::fetchConfigDescriptionFinished,
            this,
            [this](const QMap<QString, ConfigOptionList> &descs, const QString &mainType) {
                desc_ = descs;
                mainType_ = mainType;
                setupWidget(mainWidget_, mainType_, QString());
                initialized_ = true;
            });
    connect(m_worker, &IMWorker::fetchConfigValueFinished, this, [this](const QVariant &variant) {
        setValue(variant);
    });
    /* connect(m_worker, &IMWorker::fetchGroupsFinished, this, [this](const QStringList &groups)
     * {}); */
}

/* ConfigWidget::ConfigWidget(const QMap<QString, FcitxQtConfigOptionList> &desc, */
/*                            QString mainType, */
/*                            DBusProvider *dbus, */
/*                            QWidget *parent) */
/*     : QWidget(parent) */
/*     , desc_(desc) */
/*     , mainType_(mainType) */
/*     , dbus_(dbus) */
/*     , mainWidget_(new QWidget(this)) */
/* { */
/*     setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred); */
/*     QVBoxLayout *layout = new QVBoxLayout; */
/*     layout->addWidget(mainWidget_); */
/*     setLayout(layout); */
/**/
/*     setupWidget(mainWidget_, mainType_, QString()); */
/*     initialized_ = true; */
/* } */
// TODO: need to implement in other module
/* void ConfigWidget::requestConfig(bool sync) */
/* { */
/*     if (!dbus_->controller()) { */
/*         return; */
/*     } */
/*     auto call = dbus_->controller()->GetConfig(uri_); */
/*     auto watcher = new QDBusPendingCallWatcher(call, this); */
/*     connect(watcher, */
/*             &QDBusPendingCallWatcher::finished, */
/*             this, */
/*             &ConfigWidget::requestConfigFinished); */
/*     if (sync) { */
/*         watcher->waitForFinished(); */
/*     } */
/* } */
/**/
/* void ConfigWidget::requestConfigFinished(QDBusPendingCallWatcher *watcher) */
/* { */
/*     watcher->deleteLater(); */
/*     QDBusPendingReply<QDBusVariant, FcitxQtConfigTypeList> reply = *watcher; */
/*     if (reply.isError()) { */
/*         return; */
/*     } */
/**/
/*     if (!initialized_) { */
/*         auto desc = reply.argumentAt<1>(); */
/*         if (!desc.size()) { */
/*             return; */
/*         } */
/**/
/*         for (auto &type : desc) { */
/*             desc_[type.name()] = type.options(); */
/*         } */
/*         mainType_ = desc[0].name(); */
/*         setupWidget(mainWidget_, mainType_, QString()); */
/*         initialized_ = true; */
/*     } */
/**/
/*     if (initialized_) { */
/*         setValue(reply.argumentAt<0>().variant()); */
/*     } */
/**/
/*     // adjustSize(); */
/* } */

void ConfigWidget::load()
{
    // TODO: need to implement
    /* if (uri_.isEmpty()) { */
    /*     return; */
    /* } */
    /* requestConfig(); */
}

void ConfigWidget::save()
{
    // TODO: need to implement
    //
    /* if (!dbus_->controller() || uri_.isEmpty()) { */
    /*     return; */
    /* } */
    /* QDBusVariant var(value()); */
    /* dbus_->controller()->SetConfig(uri_, var); */
}

void ConfigWidget::setValue(const QVariant &value)
{
    if (!initialized_) {
        return;
    }
    dontEmitChanged_ = true;
    auto optionWidgets = findChildren<OptionWidget *>();
    QVariantMap map;
    if (value.canConvert<QDBusArgument>()) {
        auto argument = qvariant_cast<QDBusArgument>(value);
        argument >> map;
    } else {
        map = value.toMap();
    }
    for (auto optionWidget : optionWidgets) {
        optionWidget->readValueFrom(map);
    }
    dontEmitChanged_ = false;
}

QVariant ConfigWidget::value() const
{
    QVariantMap map;
    auto optionWidgets = findChildren<OptionWidget *>();
    for (auto optionWidget : optionWidgets) {
        optionWidget->writeValueTo(map);
    }
    return map;
}

void ConfigWidget::buttonClicked(QDialogButtonBox::StandardButton button)
{
    if (button == QDialogButtonBox::RestoreDefaults) {
        auto optionWidgets = findChildren<OptionWidget *>();
        for (auto optionWidget : optionWidgets) {
            optionWidget->restoreToDefault();
        }
    } else if (button == QDialogButtonBox::Ok) {
        save();
    }
}

void ConfigWidget::setupWidget(QWidget *widget, const QString &type, const QString &path)
{
    if (!desc_.contains(type)) {
        // qCCritical(KCM_FCITX5) << type << " type does not exists.";
    }

    auto layout = new QFormLayout(widget);
    auto &options = desc_[type];
    for (auto &option : options) {
        qWarning() << "11111" << type << path << option.name();
        addOptionWidget(layout,
                        option,
                        path.isEmpty() ? option.name() : path + "/" + option.name());
    }

    widget->setLayout(layout);
}

void ConfigWidget::addOptionWidget(QFormLayout *layout,
                                   const operation::ConfigOption &option,
                                   const QString &path)
{
    if (auto optionWidget = OptionWidget::addWidget(layout, option, path, this)) {
        connect(optionWidget, &OptionWidget::valueChanged, this, &ConfigWidget::doChanged);
    } else if (desc_.contains(option.type())) {
        QGroupBox *box = new QGroupBox;
        box->setTitle(option.description());
        QVBoxLayout *innerLayout = new QVBoxLayout;
        QWidget *widget = new QWidget;
        setupWidget(widget, option.type(), path);
        innerLayout->addWidget(widget);
        box->setLayout(innerLayout);
        layout->addRow(box);
    } else {
        // qCDebug(KCM_FCITX5) << "Unknown type: " << option.type();
    }
}

QDialog *ConfigWidget::configDialog(QWidget *parent,
                                    IMWorker *worker,
                                    /* DBusProvider *dbus, */
                                    const QString &uri,
                                    const QString &title)
{
    auto configPage = new ConfigWidget(worker);
    m_worker->setInitializedStatus(false);
    m_worker->fetchConfig(uri);
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults);

    buttonBox->button(QDialogButtonBox::Ok)->setText(_("&OK"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(_("&Cancel"));
    buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(_("Restore &Defaults"));

    auto configPageWrapper = new VerticalScrollArea;
    configPageWrapper->setWidget(configPage);
    dialogLayout->addWidget(configPageWrapper);
    dialogLayout->addWidget(buttonBox);

    connect(buttonBox,
            &QDialogButtonBox::clicked,
            configPage,
            [configPage, buttonBox](QAbstractButton *button) {
                configPage->buttonClicked(buttonBox->standardButton(button));
            });

    QDialog *dialog = new QDialog(parent);
    dialog->setWindowIcon(QIcon::fromTheme("fcitx"));
    dialog->setWindowTitle(title);
    dialog->setLayout(dialogLayout);
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    return dialog;
}

void ConfigWidget::doChanged()
{
    if (dontEmitChanged_) {
        return;
    }
    Q_EMIT changed();
}

/*ConfigWidget *getConfigWidget(QWidget *widget) {
    widget = widget->parentWidget();
    ConfigWidget *configWidget;
    while (widget) {
        configWidget = qobject_cast<ConfigWidget *>(widget);
        if (configWidget) {
            break;
        }
        widget = widget->parentWidget();
    }
    return configWidget;
}
*/

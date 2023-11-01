// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _KCM_FCITX_CONFIGWIDGET_H_
#define _KCM_FCITX_CONFIGWIDGET_H_

#include "operation/configoption.h"
#include "operation/imworker.h"

#include <QDialogButtonBox>
class QFormLayout;

namespace dcc_im_plugin {
namespace widgets {

class ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigWidget(dcc_im_plugin::operation::IMWorker *worker,
                          /*const QString &uri, DBusProvider *module,*/ QWidget *parent = 0);

    /* explicit ConfigWidget(const QMap<QString, operation::ConfigOption> &desc, */
    /*                       QString mainType, */
    /*                       DBusProvider *module, */
    /*                       QWidget *parent = 0); */

    static QDialog *configDialog(QWidget *parent,
                                 dcc_im_plugin::operation::IMWorker *worker,
                                 /* DBusProvider *module, */
                                 const QString &uri,
                                 const QString &title = QString());

    /* auto dbus() { return dbus_; } */

    auto &description() const { return desc_; }

Q_SIGNALS:
    void changed();

public Q_SLOTS:
    void load();
    void save();
    void buttonClicked(QDialogButtonBox::StandardButton);

    QVariant value() const;
    void setValue(const QVariant &variant);
    /* void requestConfig(bool sync = false); */
private Q_SLOTS:
    /* void requestConfigFinished(QDBusPendingCallWatcher *watcher); */
    void doChanged();

private:
    void setupWidget(QWidget *widget, const QString &type, const QString &path);
    void addOptionWidget(QFormLayout *layout,
                         const operation::ConfigOption &option,
                         const QString &path);

    bool initialized_ = false;
    /* QString uri_; */
    QMap<QString, QList<operation::ConfigOption>> desc_;
    QString mainType_;
    /* DBusProvider *dbus_; */
    QWidget *mainWidget_;

    bool dontEmitChanged_ = false;
    static operation::IMWorker *m_worker;
};
} // namespace widgets
} // namespace dcc_im_plugin

#endif // _KCM_FCITX_CONFIGWIDGET_H_

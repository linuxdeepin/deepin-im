// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TRAYICON_TRAYICON_H
#define TRAYICON_TRAYICON_H

#include <dimcore/FrontendAddon.h>

#include <QQmlApplicationEngine>

namespace org {
namespace deepin {
namespace dim {

class TrayIcon : public FrontendAddon
{
    Q_OBJECT

    Q_PROPERTY(QString imAddonIcon MEMBER imAddonIcon_ NOTIFY imAddonIconChanged)

public:
    explicit TrayIcon(Dim *dim);
    ~TrayIcon() override;

signals:
    void imAddonIconChanged(const QString &imAddonIcon);

private:
    QQmlApplicationEngine engine;

    QString imAddonIcon_;
    QMetaObject::Connection oldConnection_;

    void onFocusedInputContextChanged(int focusedInputContext);
    void onImSwitched(const std::pair<QString, QString> &imIndex);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !TRAYICON_TRAYICON_H

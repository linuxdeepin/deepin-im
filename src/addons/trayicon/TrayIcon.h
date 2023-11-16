// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TRAYICON_TRAYICON_H
#define TRAYICON_TRAYICON_H

#include <dimcore/FrontendAddon.h>

#include <QList>
#include <QQmlApplicationEngine>

namespace org {
namespace deepin {
namespace dim {

class TrayIconInputMethodEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString addon MEMBER addon_ NOTIFY addonChanged)
    Q_PROPERTY(QString name MEMBER name_ NOTIFY nameChanged)
    Q_PROPERTY(QString icon MEMBER icon_ NOTIFY iconChanged)

public:
    TrayIconInputMethodEntry(const QString &addon, const QString &name, const QString &icon);
    ~TrayIconInputMethodEntry() override;

    QString addon() const { return addon_; }

    QString name() const { return name_; }

signals:
    void addonChanged(const QString &addon);
    void nameChanged(const QString &name);
    void iconChanged(const QString &icon);

private:
    QString addon_;
    QString name_;
    QString icon_;
};

class TrayIcon : public FrontendAddon
{
    Q_OBJECT

    Q_PROPERTY(QString imAddonIcon MEMBER imAddonIcon_ NOTIFY imAddonIconChanged)
    Q_PROPERTY(QQmlListProperty<TrayIconInputMethodEntry> imEntries READ getIMEntries NOTIFY
                   imEntriesChanged)

public:
    explicit TrayIcon(Dim *dim);
    ~TrayIcon() override;

    QQmlListProperty<TrayIconInputMethodEntry> const getIMEntries();
    void const updateTrayIconIMEntries();

public:
    Q_INVOKABLE void imEntryMenuTriggered(const QString &addon, const QString &name);
    Q_INVOKABLE void configureTriggered();

signals:
    void imAddonIconChanged(const QString &imAddonIcon);
    void imEntriesChanged(QQmlListProperty<TrayIconInputMethodEntry>);

private:
    bool loadTranslator();

private:
    QQmlApplicationEngine engine;

    QString imAddonIcon_;
    QList<TrayIconInputMethodEntry *> trayIconIMEntries_;
    QMetaObject::Connection oldConnection_;

    void onFocusedInputContextChanged(int focusedInputContext);
    void onImSwitched(const std::pair<std::string, std::string> &imIndex);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !TRAYICON_TRAYICON_H

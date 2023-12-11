// Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2023 Deepin Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KETSETTINGSITEM_H
#define KETSETTINGSITEM_H

#include "DLineEdit"
#include "keylabel.h"
#include "shortenlabel.h"

#include <dtkwidget_global.h>
#include <widgets/settingsitem.h>

#include <QComboBox>
#include <QPushButton>

DWIDGET_USE_NAMESPACE

namespace dcc_im_plugin {
namespace widgets {

class KeyLabelWidget : public QWidget
{
    Q_OBJECT
public:
    KeyLabelWidget(QStringList list = {}, QWidget *p = nullptr);
    virtual ~KeyLabelWidget();
    void setKeyId(const QString &id);
    void setList(const QStringList &list);
    QString getKeyToStr();
    void setEnableEdit(bool flag);
    /**
     * @brief 设置是否可以输入单个按键
     */
    void enableSingleKey();
signals:
    void editedFinish();
    void shortCutError(const QStringList &list, QString &name);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void clearShortcutKey();
    void setShortcutShow(bool flag);
    bool checkNewKey(bool isRelease = false);
    void initLableList(const QStringList &list);

private:
    QHBoxLayout *m_mainLayout{ nullptr };
    QLineEdit *m_keyEdit{ nullptr };
    QList<KeyLabel *> m_list;
    QString m_id;
    QStringList m_curlist;
    QStringList m_newlist;
    bool m_eidtFlag;
    bool m_isSingle{ false };
};

class KeySettingsItem : public DCC_NAMESPACE::SettingsItem
{
    Q_OBJECT
public:
    KeySettingsItem(const QString &text = "",
                    const QStringList &list = {},
                    QFrame *parent = nullptr);
    void setKeyId(const QString &id);
    void setList(const QStringList &list);

    QString getKeyToStr() { return m_keyWidget->getKeyToStr(); }

    void setEnableEdit(bool flag);
    QString getLabelText();
    void setText(const QString &text);
    /**
     * @brief 设置是否可以输入单个按键
     */
    void enableSingleKey();

signals:
    void editedFinish();
    void shortCutError(const QString &curName, const QStringList &list, QString &name);

public slots:
    void doShortCutError(const QStringList &list, QString &name);

private:
    ShortenLabel *m_label{ nullptr };
    QHBoxLayout *m_hLayout{ nullptr };
    KeyLabelWidget *m_keyWidget{ nullptr };
};

class HotKeySettingsItem : public DCC_NAMESPACE::SettingsItem
{
    Q_OBJECT
public:
    HotKeySettingsItem(const QString &text = "",
                       const QStringList &list = {},
                       QFrame *parent = nullptr);
    void setKeyId(const QString &id);
    void setList(const QStringList &list);

    QString getKeyToStr() { return m_keyWidget->getKeyToStr(); }

    void setEnableEdit(bool flag);
    QString getLabelText();
    void setText(const QString &text);
    /**
     * @brief 设置是否可以输入单个按键
     */
    void enableSingleKey();

signals:
    void editedFinish();
    void shortCutError(const QString &curName, const QStringList &list, QString &name);

public slots:
    void doShortCutError(const QStringList &list, QString &name);

private:
    ShortenLabel *m_label{ nullptr };
    QHBoxLayout *m_hLayout{ nullptr };
    KeyLabelWidget *m_keyWidget{ nullptr };
};

} // namespace widgets
} // namespace dcc_im_plugin
#endif // KETSETTINGSITEM_H

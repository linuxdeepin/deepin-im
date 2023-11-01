// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keysettingsitem.h"

#include "utils/utilfuncs.h"

#include <DCommandLinkButton>
#include <DFontSizeManager>

#include <QBrush>
#include <QCheckBox>
#include <QComboBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainterPath>
#include <QProcess>

using namespace dcc_im_plugin::widgets;

KeyLabelWidget::KeyLabelWidget(QStringList list, QWidget *p)
    : QWidget(p)
    , m_curlist(list)
{
    m_eidtFlag = true;
    if (m_curlist.isEmpty()) {
        m_curlist << tr("None");
    }
    m_keyEdit = new QLineEdit(this);
    m_keyEdit->installEventFilter(this);
    m_keyEdit->setReadOnly(true);
    m_keyEdit->hide();
    m_keyEdit->setPlaceholderText(tr("Enter a new shortcut"));
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 9, 0, 9);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_keyEdit);
    setLayout(m_mainLayout);
    setList(m_curlist);
    setShortcutShow(true);
}

KeyLabelWidget::~KeyLabelWidget()
{
    clearShortcutKey();
}

void KeyLabelWidget::setKeyId(const QString &id)
{
    m_id = id;
}

void KeyLabelWidget::setList(const QStringList &list)
{
    m_curlist = list;
    m_curlist.removeDuplicates();
    initLableList(m_curlist);
}

void KeyLabelWidget::initLableList(const QStringList &list)
{
    // qDebug() << "initLableList " << list;
    clearShortcutKey();
    for (const QString &key : list) {
        QString tmpKey = key.toLower();
        if (tmpKey.compare("control") == 0) {
            tmpKey = "ctrl";
        }
        if (!tmpKey.isEmpty()) {
            tmpKey[0] = tmpKey[0].toUpper();
        }
        KeyLabel *label = new KeyLabel(tmpKey);
        label->setAccessibleName(tmpKey);
        label->setBackgroundRole(DPalette::DarkLively);
        m_list << label;
        m_mainLayout->addWidget(label);
    }
    // qDebug() << "m_curlist " << m_curlist;
    // qDebug() << "m_newlist " << m_newlist;
}

QString KeyLabelWidget::getKeyToStr()
{
    QString key;
    for (int i = 0; i < m_list.count(); ++i) {
        if (i == m_list.count() - 1) {
            key += m_list[i]->text();
        } else {
            key += (m_list[i]->text() + "_");
        }
    }
    return key.toUpper();
}

void KeyLabelWidget::setEnableEdit(bool flag)
{
    m_eidtFlag = flag;
}

void KeyLabelWidget::enableSingleKey()
{
    m_isSingle = true;
}

void KeyLabelWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_eidtFlag)
        return;
    setShortcutShow(!m_keyEdit->isHidden());
    QWidget::mousePressEvent(event);
}

bool KeyLabelWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (m_keyEdit == watched) {
        if (event->type() == QEvent::Hide || event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::FocusOut) {
            setShortcutShow(true);
            return true;
        }
        if (event->type() == QEvent::Show) {
            setShortcutShow(false);
            return true;
        }
        if (event->type() == QEvent::KeyPress) {
            // Dynamic_Cast(QKeyEvent, e, event);
            QKeyEvent *e = dynamic_cast<QKeyEvent *>(event);

            auto func = [=](QStringList &list, const QString &key) {
                clearShortcutKey();
                list.clear();
                //                if((key == "Ctrl" && m_curlist.contains("CTRL",
                //                Qt::CaseInsensitive)) || (key == "Alt" &&
                //                m_curlist.contains("ALT", Qt::CaseInsensitive))){
                //                    return;
                //                }
                list << key;
                qDebug() << "func: " << m_curlist;
                initLableList(list);
                setShortcutShow(true);
            };

            if (e) {
                if (e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) {
                    func(m_curlist, tr("None"));

                } else if (e->key() == Qt::Key_Control || e->key() == Qt::Key_Alt || m_isSingle) {
                    setFocus();
                    func(m_newlist, utilFuncs::getKeyValue(e->key()));
                } else {
                    setShortcutShow(true);
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

void KeyLabelWidget::keyPressEvent(QKeyEvent *event)
{
    if (!m_eidtFlag)
        return;
    quint32 tmpScanCode = event->nativeScanCode();
    if (tmpScanCode == 64) {
        m_newlist << utilFuncs::getKeyValue(Qt::Key_Alt);
    } else {
        m_newlist << utilFuncs::getKeyValue(event->key());
    }
    initLableList(m_newlist);
    if (m_newlist.count() >= 2 && !checkNewKey()) {
        initLableList(m_curlist);
        qDebug() << "m_newlist.count() >= 2 && !checkNewKey()";
    }
    setShortcutShow(true);
    QWidget::keyPressEvent(event);
}

void KeyLabelWidget::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "keyReleaseEvent";
    if (!m_eidtFlag)
        return;
    if ((m_newlist.count() < 2 && !m_isSingle) || !checkNewKey(true)) {
        m_curlist.removeDuplicates();
        initLableList(m_curlist);
    }
    setShortcutShow(true);
    //    QWidget::keyReleaseEvent(event);
}

void KeyLabelWidget::clearShortcutKey()
{
    for (KeyLabel *label : m_list) {
        m_mainLayout->removeWidget(label);
        label->deleteLater();
    }
    m_list.clear();
}

void KeyLabelWidget::setShortcutShow(bool flag)
{
    if (flag) {
        m_mainLayout->setContentsMargins(0, 9, 0, 9);
        m_keyEdit->hide();
        int w = 0;
        for (KeyLabel *label : m_list) {
            w += label->width() + 9;
            label->show();
        }
        setMaximumWidth(w);
        setFocus();
    } else {
        for (KeyLabel *label : m_list) {
            label->hide();
        }
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_keyEdit->show();
        m_keyEdit->setFocus();
        m_keyEdit->clear();
        setMaximumWidth(9999);
    }
    update();
}

bool KeyLabelWidget::checkNewKey(bool isRelease)
{
    QStringList list{ utilFuncs::getKeyValue(Qt::Key_Control),
                      utilFuncs::getKeyValue(Qt::Key_Alt),
                      utilFuncs::getKeyValue(Qt::Key_Shift),
                      utilFuncs::getKeyValue(Qt::Key_Super_L) };

    if (m_newlist.count() == 2) {
        for (int i = 0; i < list.count(); ++i) {
            if (m_newlist.at(0) == list.at(i)) {
                if (list.indexOf(m_newlist[1]) != -1) {
                    if (m_newlist[1] != m_newlist[0]) {
                        return !isRelease;
                    }
                    return false;
                }
                if (list.indexOf(m_newlist[1]) == -1) {
                    QStringList tmpList;
                    for (const QString &key : m_newlist) {
                        QString tmpKey = key.toUpper();
                        tmpList.append(tmpKey);
                    }

                    QString configName;
                    if (m_curlist == tmpList) {
                        emit shortCutError(m_newlist, configName);
                        return false;
                    }
                    setList(m_newlist);
                    focusNextChild();
                    emit editedFinish();
                    return true;
                }
            }
        }
    }
    if (m_newlist.count() >= 3) {
        if (list.indexOf(m_newlist[0]) == -1 || list.indexOf(m_newlist[1]) == -1
            || list.indexOf(m_newlist[2]) != -1) {
            focusNextChild();
            return false;
        }
        QStringList tmpList;
        for (const QString &key : m_newlist) {
            QString tmpKey = key.toUpper();
            tmpList.append(tmpKey);
        }
        QString configName;
        if (m_curlist != tmpList /*&& !IMConfig::checkShortKey(m_newlist, configName)*/) {
            emit shortCutError(m_newlist, configName);
            return false;
        }
        setList(m_newlist);
        focusNextChild();
        emit editedFinish();
        return true;
    }
    if (m_newlist.count() == 1 && m_isSingle) {
        emit editedFinish();
    }
    return true;
}

KeySettingsItem::KeySettingsItem(const QString &text, const QStringList &list, QFrame *parent)
    : SettingsItem(parent)
{
    m_label = new ShortenLabel(text, this);
    m_keyWidget = new KeyLabelWidget(list, parent);
    m_hLayout = new QHBoxLayout(this);
    m_hLayout->setContentsMargins(10, 0, 10, 0);
    m_hLayout->addWidget(m_label);
    m_hLayout->addWidget(m_keyWidget);
    m_hLayout->setAlignment(m_label, Qt::AlignLeft);
    m_hLayout->addWidget(m_keyWidget, 0, Qt::AlignVCenter | Qt::AlignRight);
    setFixedHeight(48);
    setAccessibleName(text);
    setLayout(m_hLayout);
    connect(m_keyWidget, &KeyLabelWidget::editedFinish, this, &KeySettingsItem::editedFinish);
    connect(m_keyWidget, &KeyLabelWidget::shortCutError, this, &KeySettingsItem::doShortCutError);
}

void KeySettingsItem::setText(const QString &text)
{
    m_label->setShortenText(text);
}

void KeySettingsItem::enableSingleKey()
{
    m_keyWidget->enableSingleKey();
}

QString KeySettingsItem::getLabelText()
{
    return m_label->text();
}

void KeySettingsItem::setEnableEdit(bool flag)
{
    m_keyWidget->setEnableEdit(flag);
}

void KeySettingsItem::setKeyId(const QString &id)
{
    m_keyWidget->setKeyId(id);
}

void KeySettingsItem::setList(const QStringList &list)
{
    m_keyWidget->setList(list);
}

void KeySettingsItem::doShortCutError(const QStringList &list, QString &name)
{
    emit KeySettingsItem::shortCutError(m_label->text(), list, name);
}

HotKeySettingsItem::HotKeySettingsItem(const QString &text, const QStringList &list, QFrame *parent)
    : SettingsItem(parent)
{
    m_label = new ShortenLabel(text, this);
    m_keyWidget = new KeyLabelWidget(list, parent);
    m_hLayout = new QHBoxLayout(this);
    m_hLayout->setContentsMargins(10, 0, 10, 0);
    m_hLayout->addWidget(m_label);
    m_hLayout->addWidget(m_keyWidget);
    m_hLayout->setAlignment(m_label, Qt::AlignLeft);
    m_hLayout->addWidget(m_keyWidget, 0, Qt::AlignVCenter | Qt::AlignRight);
    setFixedHeight(48);
    setAccessibleName(text);
    setLayout(m_hLayout);
    connect(m_keyWidget, &KeyLabelWidget::editedFinish, this, &HotKeySettingsItem::editedFinish);
    connect(m_keyWidget,
            &KeyLabelWidget::shortCutError,
            this,
            &HotKeySettingsItem::doShortCutError);
}

void HotKeySettingsItem::setText(const QString &text)
{
    m_label->setShortenText(text);
}

void HotKeySettingsItem::enableSingleKey()
{
    m_keyWidget->enableSingleKey();
}

QString HotKeySettingsItem::getLabelText()
{
    return m_label->text();
}

void HotKeySettingsItem::setEnableEdit(bool flag)
{
    m_keyWidget->setEnableEdit(flag);
}

void HotKeySettingsItem::setKeyId(const QString &id)
{
    m_keyWidget->setKeyId(id);
}

void HotKeySettingsItem::setList(const QStringList &list)
{
    m_keyWidget->setList(list);
}

void HotKeySettingsItem::doShortCutError(const QStringList &list, QString &name)
{
    emit HotKeySettingsItem::shortCutError(m_label->text(), list, name);
}

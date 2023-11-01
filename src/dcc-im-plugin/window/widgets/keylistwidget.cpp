// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keylistwidget.h"

#include "keysequencewidget.h"
#include "utils/translate.h"

#include <qabstractbutton.h>

#include <QHBoxLayout>
#include <QToolButton>
#include <QVBoxLayout>

using namespace dcc_im_plugin::widgets;
using namespace utilFuncs;

KeyListWidget::KeyListWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout;
    layout->setMargin(0);
    keysLayout_ = new QVBoxLayout;
    keysLayout_->setMargin(0);
    auto subLayout = new QVBoxLayout;

    addButton_ = new QToolButton;
    addButton_->setAutoRaise(true);
    addButton_->setIcon(QIcon::fromTheme("list-add-symbolic"));
    addButton_->setText(_("Add"));
    addButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(addButton_, &QToolButton::clicked, this, [this]() {
        addKey(Key());
        emit keyChanged();
    });

    layout->addLayout(keysLayout_);
    subLayout->addWidget(addButton_, 0, Qt::AlignTop);
    // subLayout->addStretch(1);
    layout->addLayout(subLayout);

    setLayout(layout);

    // Add an empty one.
    addKey();
}

void KeyListWidget::addKey(Key key)
{
    KeySequenceWidget *keyWidget = new KeySequenceWidget;
    keyWidget->setClearButtonShown(false);
    keyWidget->setKeySequence({ key });
    keyWidget->setModifierlessAllowed(modifierLess_);
    keyWidget->setModifierOnlyAllowed(modifierOnly_);
    auto widget = new QWidget;
    auto layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(keyWidget);
    auto removeButton = new QToolButton;
    removeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    removeButton->setIcon(QIcon::fromTheme("list-remove-symbolic"));
    /* removeButton->setText(_("Remove")); */
    // TODO: need to implement the translation
    removeButton->setText("Remove");
    removeButton->setVisible(showRemoveButton());
    layout->addWidget(removeButton);
    widget->setLayout(layout);
    connect(removeButton, &QAbstractButton::clicked, widget, [widget, this]() {
        auto idx = keysLayout_->indexOf(widget);
        if (removeKeyAt(idx)) {
            emit keyChanged();
        }
    });
    connect(keyWidget, &KeySequenceWidget::keySequenceChanged, this, &KeyListWidget::keyChanged);
    connect(this, &KeyListWidget::keyChanged, removeButton, [this, removeButton]() {
        removeButton->setVisible(showRemoveButton());
    });
    keysLayout_->addWidget(widget);
}

void KeyListWidget::setKeys(const QList<Key> &keys)
{
    while (keysLayout_->count() > 1) {
        removeKeyAt(0);
    }
    removeKeyAt(0);
    QString s = QString::fromUtf8(Key::keyListToString(keys, KeyStringFormat::Portable).c_str());
    bool first = true;
    for (auto key : keys) {

        if (first) {
            first = false;
            keysLayout_->itemAt(0)->widget()->findChild<KeySequenceWidget *>()->setKeySequence(
                { key });
        } else {
            addKey(key);
        }
    }
    emit keyChanged();
}

QList<Key> KeyListWidget::keys() const
{
    QList<Key> result;
    for (int i = 0; i < keysLayout_->count(); i++) {
        if (auto keyWidget = keysLayout_->itemAt(i)->widget()->findChild<KeySequenceWidget *>()) {
            if (keyWidget->keySequence().isEmpty()) {
                continue;
            }
            auto &key = keyWidget->keySequence()[0];
            if (key.isValid() && !result.contains(key)) {
                result << keyWidget->keySequence()[0];
            }
        }
    }
    return result;
}

void KeyListWidget::setAllowModifierLess(bool value)
{
    if (value == modifierLess_) {
        return;
    }

    modifierLess_ = value;

    for (int i = 0; i < keysLayout_->count(); i++) {
        if (auto keyWidget = keysLayout_->itemAt(i)->widget()->findChild<KeySequenceWidget *>()) {
            keyWidget->setModifierlessAllowed(modifierLess_);
        }
    }
}

void KeyListWidget::setAllowModifierOnly(bool value)
{
    if (value == modifierOnly_) {
        return;
    }

    modifierOnly_ = value;

    for (int i = 0; i < keysLayout_->count(); i++) {
        if (auto keyWidget = keysLayout_->itemAt(i)->widget()->findChild<KeySequenceWidget *>()) {
            keyWidget->setModifierOnlyAllowed(modifierOnly_);
        }
    }
}

bool KeyListWidget::removeKeyAt(int idx)
{
    if (idx < 0 || idx > keysLayout_->count()) {
        return false;
    }
    auto widget = keysLayout_->itemAt(idx)->widget();
    if (keysLayout_->count() == 1) {
        keysLayout_->itemAt(0)->widget()->findChild<KeySequenceWidget *>()->setKeySequence(
            QList<Key>());
    } else {
        keysLayout_->removeWidget(widget);
        delete widget;
    }
    return true;
}

bool KeyListWidget::showRemoveButton() const
{
    return keysLayout_->count() > 1
        || (keysLayout_->count() == 1
            && keysLayout_->itemAt(0)
                   ->widget()
                   ->findChild<KeySequenceWidget *>()
                   ->keySequence()
                   .size());
}

void KeyListWidget::resizeEvent(QResizeEvent *event)
{
    if (keysLayout_->count() > 0) {
        addButton_->setMinimumHeight(
            keysLayout_->itemAt(0)->widget()->findChild<KeySequenceWidget *>()->height());
        addButton_->setMaximumHeight(addButton_->minimumHeight());
    }

    QWidget::resizeEvent(event);
}

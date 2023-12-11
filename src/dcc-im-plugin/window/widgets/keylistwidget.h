// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "listoptionwidget.h"
#include "utils/key.h"
#include "varianthelper.h"

#include <QAbstractListModel>
#include <QDebug>
#include <QtGlobal>

#ifndef KEYLISTWIDGET_H_
#  define KEYLISTWIDGET_H_

#  include <QWidget>

class QToolButton;
class QBoxLayout;
using namespace dcc_im_plugin::utils;

namespace dcc_im_plugin {
namespace widgets {

class KeyListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyListWidget(QWidget *parent = 0);

    QList<Key> keys() const;
    void setKeys(const QList<Key> &keys);
    void setAllowModifierLess(bool);
    void setAllowModifierOnly(bool);

signals:
    void keyChanged();

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    void addKey(Key key = Key());
    bool removeKeyAt(int idx);
    bool showRemoveButton() const;

    QToolButton *addButton_;
    QBoxLayout *keysLayout_;
    bool modifierLess_ = false;
    bool modifierOnly_ = false;
};
} // namespace widgets
} // namespace dcc_im_plugin

#endif // KEYLISTWIDGET_H_

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMWINDOW_H
#define IMWINDOW_H

#include "interface/namespace.h"
#include "operation/inputmethodentry.h"

#include <dtkwidget_global.h>

#include <QVBoxLayout>
#include <QWidget>

DWIDGET_BEGIN_NAMESPACE
class DIconButton;
class DCommandLinkButton;
class DListView;
DWIDGET_END_NAMESPACE

namespace DCC_NAMESPACE {
class DCCListView;
} // namespace DCC_NAMESPACE

class QStandardItemModel;

namespace dcc_im_plugin {

class IMWindow : public QWidget
{
    Q_OBJECT
public:
    explicit IMWindow(QWidget *parent = nullptr);
    ~IMWindow() = default;

public Q_SLOTS:
    void updateEnabledInputMethodEntryList(const operation::InputMethodEntryList &imDataList);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initShortCut();
    void initAdvanceConfig();
    void initCustomConfig();
    void initMainUI();
    void initIMList();
    void onItemUp(const int index);
    void onItemDown(const int index);
    void onItemConfig(const int index);
    void updateActions();

private:
    dccV23::DCCListView *m_IMListView = nullptr;
    QStandardItemModel *m_IMListModel = nullptr;
    QVBoxLayout *m_managerConfigLayout = nullptr;
    QVBoxLayout *m_shortCutConfigLayout = nullptr;
    QVBoxLayout *m_advancedConfigLayout = nullptr;
    QVBoxLayout *m_customConfigLayout = nullptr;
    Dtk::Widget::DIconButton *m_deleteBtn = nullptr;
    int m_hoveredRow = -1;
};

} // namespace dcc_im_plugin
#endif // DEBUG

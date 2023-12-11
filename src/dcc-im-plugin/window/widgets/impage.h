// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef IMPAGE_H
#define IMPAGE_H

#include "interface/namespace.h"
#include "operation/inputmethodentry.h"

#include <dtkwidget_global.h>

#include <DAbstractDialog>

class QFrame;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QStandardItemModel;
class QSortFilterProxyModel;

DWIDGET_BEGIN_NAMESPACE
class DCommandLinkButton;
class DListView;
class DSearchEdit;
DWIDGET_END_NAMESPACE;

namespace DCC_NAMESPACE {
class ButtonTuple;
}

namespace dcc_im_plugin {
namespace operation {
class IMWorker;
}

namespace widgets {
class KeyboardWidget;
}
} // namespace dcc_im_plugin

namespace dcc_im_plugin {
namespace widgets {

class IMPage : public DTK_WIDGET_NAMESPACE::DAbstractDialog
{
    Q_OBJECT
public:
    IMPage(operation::IMWorker *worker, QWidget *parent = nullptr);
    ~IMPage();
signals:
    void changed();
    void closeAddIMWindow();

public slots:
    void save();
    void load();
    void defaults();

private slots:
    void availIMCurrentChanged(const QModelIndex &index);
    void childIMSelectionChanged();

    void clickedFindMoreButton();

    void clickedCloseButton();
    void clickedAddButton();

private:
    void updateAvailableIMList();

private:
    QHBoxLayout *m_contentLayout = nullptr;
    QVBoxLayout *m_leftLayout = nullptr;
    Dtk::Widget::DSearchEdit *m_searchEdit = nullptr;
    QFrame *line = nullptr;
    Dtk::Widget::DListView *m_availIMList = nullptr;
    QVBoxLayout *m_rightLayout = nullptr;
    Dtk::Widget::DListView *m_childIMList = nullptr;
    KeyboardWidget *m_laSelector = nullptr;
    Dtk::Widget::DCommandLinkButton *m_findMoreLabel = nullptr;
    DCC_NAMESPACE::ButtonTuple *m_buttonTuple = nullptr;
    operation::IMWorker *m_worker = nullptr;
    QStandardItemModel *m_model = nullptr;
    QStandardItemModel *m_childModel = nullptr;
    QSortFilterProxyModel *m_proxyModel = nullptr;
    QSortFilterProxyModel *m_proxyChildModel = nullptr;
    QMap<QString, operation::InputMethodEntryList> m_filteredIMEntryList;
};

} // namespace widgets
} // namespace dcc_im_plugin

#endif

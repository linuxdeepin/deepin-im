// SPDX-FileCopyrightText: 2023 Deepin Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#include "imwindow.h"

#include "operation/imworker.h"
#include "widgets/configwidget.h"
#include "widgets/impage.h"
#include "widgets/keysettingsitem.h"
#include "widgets/labellinkitem.h"
#include "widgets/settingsgroup.h"

#include <dwidgetutil.h>
#include <widgets/comboxwidget.h>
#include <widgets/dcclistview.h>
#include <widgets/settingshead.h>
#include <widgets/titlelabel.h>

#include <DIconButton>

#include <QPointer>

using namespace dcc_im_plugin;
using namespace dcc_im_plugin::operation;
using namespace DCC_NAMESPACE;
DWIDGET_USE_NAMESPACE

IMWindow::IMWindow(QWidget *parent)
    : QWidget(parent)
{
    initIMList();
    initShortCut();
    initAdvanceConfig();
    initCustomConfig();
    initMainUI();
}

bool IMWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_IMListView) {
        switch (event->type()) {
        case QEvent::HoverLeave:
            m_hoveredRow = -1;
            updateActions();
            break;
        case QEvent::HoverMove: {
            auto *he = dynamic_cast<QHoverEvent *>(event);
            int newRow = m_IMListView->indexAt(he->pos()).row();
            if (newRow != m_hoveredRow) {
                m_hoveredRow = newRow;
                updateActions();
            }

            break;
        }
        default:
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void IMWindow::updateEnabledInputMethodEntryList(const operation::InputMethodEntryList &imEntryList)
{
    qWarning() << "222222" << imEntryList.size();
    m_deleteBtn->setEnabled(false);
    m_IMListModel->clear();

    for (int i = 0; i < imEntryList.count(); ++i) {
        DStandardItem *item = new DStandardItem();

        DViewItemAction *textAction = new DViewItemAction(Qt::AlignVCenter);
        textAction->setText(imEntryList.at(i).name());

        DViewItemAction *typeAction = new DViewItemAction(Qt::AlignVCenter);
        // TODO: add type
        typeAction->setText("ibus or fcitx");
        typeAction->setFontSize(DFontSizeManager::T9);
        typeAction->font().setWeight(99);

        item->setActionList(Qt::LeftEdge, DViewItemActionList() << textAction << typeAction);

        auto *upAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
        upAction->setIcon(QIcon::fromTheme("arrow_up"));
        upAction->setVisible(false);
        upAction->setDisabled(i == 0);

        auto *downAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
        downAction->setIcon(QIcon::fromTheme("arrow_down"));
        downAction->setVisible(false);
        downAction->setDisabled(i == imEntryList.count() - 1);

        auto *configAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
        configAction->setIcon(QIcon::fromTheme("setting"));
        configAction->setVisible(false);

        item->setActionList(Qt::RightEdge, { upAction, downAction, configAction });

        connect(upAction, &DViewItemAction::triggered, this, [this, item]() {
            onItemUp(item->index().row());
        });

        connect(downAction, &DViewItemAction::triggered, this, [this, item]() {
            onItemDown(item->index().row());
        });

        connect(configAction, &DViewItemAction::triggered, this, [this, i]() {
            onItemConfig(i);
        });

        item->setData(imEntryList.at(i).uniqueName(), operation::IMUniqueNameRole);
        m_IMListModel->appendRow(item);
    }
}

void IMWindow::initIMList()
{
    QHBoxLayout *managerTitleLayout = new QHBoxLayout;

    SettingsHead *imManagerTitleContext = new SettingsHead();
    imManagerTitleContext->setEditEnable(false);
    imManagerTitleContext->setTitle(tr("Manage Input Method"));
    imManagerTitleContext->layout()->setContentsMargins(10, 4, 10, 0);

    DIconButton *addBtn = new DIconButton(DStyle::SP_IncreaseElement);
    m_deleteBtn = new DIconButton(DStyle::SP_DecreaseElement);
    m_deleteBtn->setEnabled(false);

    managerTitleLayout->addWidget(imManagerTitleContext);
    managerTitleLayout->addStretch();
    managerTitleLayout->addWidget(m_deleteBtn);
    managerTitleLayout->addWidget(addBtn);

    m_IMListView = new DCCListView;
    m_IMListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_IMListView->setBackgroundType(DStyledItemDelegate::BackgroundType::ClipCornerBackground);
    m_IMListView->setSpacing(0);
    m_IMListView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_IMListView->setAttribute(Qt::WA_Hover, true);
    m_IMListView->installEventFilter(this);

    m_IMListModel = new QStandardItemModel(this);
    m_IMListView->setModel(m_IMListModel);

    widgets::LabelLinkItem *downLoadLinkItem =
        new widgets::LabelLinkItem(tr("Keyboard layout and input method management, compatible "
                                      "with Fcitx5 and IBus input method installation."),
                                   tr("Find more in App Store"));
    m_managerConfigLayout = new QVBoxLayout;
    m_managerConfigLayout->setSpacing(0);
    m_managerConfigLayout->setMargin(0);
    m_managerConfigLayout->addLayout(managerTitleLayout);
    m_managerConfigLayout->addSpacing(17);
    m_managerConfigLayout->addWidget(m_IMListView);
    m_managerConfigLayout->addSpacing(4);
    m_managerConfigLayout->addWidget(downLoadLinkItem);

    QObject::connect(m_IMListView,
                     qOverload<const QModelIndex &>(&DListView::currentChanged),
                     this,
                     [this] {
                         updateActions();
                     });
    connect(addBtn, &QAbstractButton::clicked, this, [this] {
        widgets::IMPage page(IMWorker::instance(), this);
        moveToCenter(&page);
        page.exec();
    });
    connect(IMWorker::instance(),
            &operation::IMWorker::enabledIMEntriesChanged,
            this,
            &IMWindow::updateEnabledInputMethodEntryList);
    connect(downLoadLinkItem, &widgets::LabelLinkItem::linkButtonCLicked, this, [this] {
        IMWorker::instance()->launchStore();
    });
}

void IMWindow::initShortCut()
{
    TitleLabel *imShortCutTitleLabel = new TitleLabel(tr("Shortcuts"));
    imShortCutTitleLabel->setContentsMargins(10, 0, 0, 0);

    DCommandLinkButton *resetBtn = new DCommandLinkButton(tr("Restore Defaults"));

    QHBoxLayout *shortCutTitleLayout = new QHBoxLayout;
    shortCutTitleLayout->addWidget(imShortCutTitleLabel);
    shortCutTitleLayout->addStretch();
    shortCutTitleLayout->addWidget(resetBtn);

    SettingsGroup *shortCutGroup = new SettingsGroup(nullptr, SettingsGroup::GroupBackground);
    ComboxWidget *shortCutCombo = new ComboxWidget(tr("Scroll between input methods"));
    shortCutCombo->setComboxOption({ "CTRL_SHIFT", "ALT_SHIFT", "CTRL_SUPER", "ALT_SUPER" });
    widgets::KeySettingsItem *defaultIMKey =
        new widgets::KeySettingsItem(tr("Switch between the current/first input method"));

    shortCutGroup->appendItem(shortCutCombo);
    shortCutGroup->appendItem(defaultIMKey);

    m_shortCutConfigLayout = new QVBoxLayout;
    m_shortCutConfigLayout->setSpacing(0);
    m_shortCutConfigLayout->setMargin(0);
    m_shortCutConfigLayout->addLayout(shortCutTitleLayout);
    m_shortCutConfigLayout->addSpacing(10);
    m_shortCutConfigLayout->addWidget(shortCutGroup);
}

void IMWindow::initAdvanceConfig()
{
    SettingsHead *advancedHead = new SettingsHead();
    advancedHead->setEditEnable(false);
    advancedHead->setTitle(tr("Advanced settings"));
    advancedHead->layout()->setContentsMargins(10, 4, 10, 0);
    DTipLabel *advancedConfigTip =
        new DTipLabel(tr("System-compatible advanced settings for the IBus and Fcitx5 frameworks "
                         "with settings for different frameworks"));
    advancedConfigTip->setContentsMargins(10, 0, 0, 0);
    DCCListView *advancedConfigView = new DCCListView;
    advancedConfigView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    advancedConfigView->setBackgroundType(
        DStyledItemDelegate::BackgroundType::ClipCornerBackground);
    advancedConfigView->setSpacing(0);

    QStandardItemModel *model = new QStandardItemModel(this);
    advancedConfigView->setModel(model);

    DStandardItem *ibusConfigItem = new DStandardItem;
    ibusConfigItem->setText(tr("IBus settings"));

    DViewItemAction *ibusConfigActon = new DViewItemAction;
    ibusConfigActon->setIcon(
        DStyle::standardIcon(advancedConfigView->style(), DStyle::SP_ArrowEnter));
    ibusConfigItem->setActionList(Qt::RightEdge, DViewItemActionList() << ibusConfigActon);

    DStandardItem *fcitxConfigItem = new DStandardItem;
    fcitxConfigItem->setText(tr("Fcitx5 settings"));

    DViewItemAction *fcitxConfigAction = new DViewItemAction;
    fcitxConfigAction->setIcon(
        DStyle::standardIcon(advancedConfigView->style(), DStyle::SP_ArrowEnter));
    fcitxConfigItem->setActionList(Qt::RightEdge, DViewItemActionList() << fcitxConfigAction);

    model->appendRow(ibusConfigItem);
    model->appendRow(fcitxConfigItem);

    m_advancedConfigLayout = new QVBoxLayout;
    m_advancedConfigLayout->setSpacing(0);
    m_advancedConfigLayout->setMargin(0);
    m_advancedConfigLayout->addWidget(advancedHead);
    m_advancedConfigLayout->addSpacing(4);
    m_advancedConfigLayout->addWidget(advancedConfigTip, 0, Qt::AlignLeft);
    m_advancedConfigLayout->addSpacing(10);
    m_advancedConfigLayout->addWidget(advancedConfigView);
}

void IMWindow::initCustomConfig()
{
    SettingsHead *customHead = new SettingsHead();
    customHead->setEditEnable(false);
    customHead->setTitle(tr("Custom settings"));
    customHead->layout()->setContentsMargins(10, 4, 10, 0);
    DTipLabel *customConfigTip = new DTipLabel(
        tr("Manage the keyboard layout and input method types you use in different windows"));
    customConfigTip->setContentsMargins(10, 0, 0, 0);
    SettingsGroup *windowInputSourceManagerGroup =
        new SettingsGroup(nullptr, SettingsGroup::GroupBackground);
    ComboxWidget *windowInputSourceCombo = new ComboxWidget(tr("Scroll between input methods"));
    windowInputSourceCombo->setComboxOption(
        { tr("All windows use the same source"), tr("Different windows use their own sources") });
    windowInputSourceManagerGroup->appendItem(windowInputSourceCombo);

    m_customConfigLayout = new QVBoxLayout;
    m_customConfigLayout->setSpacing(0);
    m_customConfigLayout->setMargin(0);
    m_customConfigLayout->addWidget(customHead);
    m_customConfigLayout->addSpacing(4);
    m_customConfigLayout->addWidget(customConfigTip, 0, Qt::AlignLeft);
    m_customConfigLayout->addSpacing(10);
    m_customConfigLayout->addWidget(windowInputSourceManagerGroup);
}

void IMWindow::initMainUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(m_managerConfigLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(m_shortCutConfigLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(m_advancedConfigLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(m_customConfigLayout);
    mainLayout->addSpacing(20);
    mainLayout->addStretch();
};

void IMWindow::onItemUp(const int row)
{
    if (row == 0) {
        return;
    }
    QList<QStandardItem *> items = m_IMListModel->takeRow(row);
    m_IMListModel->insertRow(row - 1, items);
    m_IMListModel->submit();
    m_IMListView->setCurrentIndex(m_IMListModel->index(row - 1, 0));
}

void IMWindow::onItemDown(const int row)
{
    if (row == m_IMListModel->rowCount() - 1) {
        return;
    }
    QList<QStandardItem *> items = m_IMListModel->takeRow(row);
    m_IMListModel->insertRow(row + 1, items);
    m_IMListModel->submit();
    m_IMListView->setCurrentIndex(m_IMListModel->index(row + 1, 0));
}

void IMWindow::onItemConfig(const int index)
{
    auto item = m_IMListModel->index(index, 0);
    QString uniqueName = item.data(operation::IMUniqueNameRole).toString();
    QString title = item.data(Qt::DisplayRole).toString();
    QPointer<QDialog> dlg = widgets::ConfigWidget::configDialog(
        this,
        IMWorker::instance(),
        QString("fcitx://config/inputmethod/%1").arg(uniqueName),
        title);
    dlg->exec();
    delete dlg;
}

void IMWindow::updateActions()
{
    auto selections = m_IMListView->selectionModel()->selectedIndexes();

    for (int i = 0; i < m_IMListModel->rowCount(); ++i) {
        auto *item = dynamic_cast<DStandardItem *>(m_IMListModel->item(i));
        if (!item)
            continue;
        auto actions = item->actionList(Qt::Edge::RightEdge);
        if (m_hoveredRow == i || selections.contains(item->index())) {
            for (auto *action : actions) {
                action->setVisible(true);
            }
        } else {
            for (auto *action : actions) {
                action->setVisible(false);
            }
        }
    }
}

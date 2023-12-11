// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "impage.h"

#include "keyboardwidget.h"
#include "operation/imworker.h"
#include "operation/inputmethodentry.h"

#include <dstyleditemdelegate.h>
#include <dstyleoption.h>
#include <widgets/buttontuple.h>

#include <DCommandLinkButton>
#include <DListView>
#include <DPaletteHelper>
#include <DSearchEdit>
#include <DTitlebar>

#include <QCollator>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QVariant>

DWIDGET_USE_NAMESPACE
using namespace DCC_NAMESPACE;
using namespace dcc_im_plugin::widgets;
using namespace dcc_im_plugin::operation;

class IMProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    IMProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        setDynamicSortFilter(true);
        sort(0);
    }

    void setFilterText(const QString &text);
    void setLanguages(const QStringList &languages);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    bool filterLanguage(const QModelIndex &index) const;
    bool filterIM(const InputMethodEntry &entry) const;
    int compareCategories(const QModelIndex &left, const QModelIndex &right) const;
    int compareItems(const QModelIndex &left, const QModelIndex &right) const;

    bool m_showOnlyCurrentLanguage = false;
    QString m_filterText;
};

void IMProxyModel::setFilterText(const QString &text)
{
    if (m_filterText != text) {
        m_filterText = text;
        invalidate();
    }
}

bool IMProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    if (index.data(RowTypeRole) == LanguageType) {
        return filterLanguage(index);
    }

    InputMethodEntry entry;
    const auto values = IMWorker::instance()->availableIMEntries().values();
    for (const InputMethodEntryList &value : values) {
        if (!entry.name().isEmpty()) {
            break;
        }
        for (const InputMethodEntry &e : value) {
            if (e.name() == index.data(Qt::DisplayRole)) {
                entry = e;
                break;
            }
        }
    }
    return filterIM(entry);
}

bool IMProxyModel::filterLanguage(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return false;
    }

    const InputMethodEntryList &entries =
        IMWorker::instance()->availableIMEntries().value(index.data(Qt::DisplayRole).toString());
    for (const InputMethodEntry &entry : entries) {
        if (filterIM(entry)) {
            return true;
        }
    }

    return false;
}

bool IMProxyModel::filterIM(const InputMethodEntry &entry) const
{
    QString uniqueName = entry.uniqueName();
    QString name = entry.name();
    QString langCode = entry.languageCode();
    // Always show keyboard us if we are not searching.
    if (uniqueName == "keyboard-us" && m_filterText.isEmpty()) {
        return true;
    }

    bool flag = true;
    QString lang = langCode.left(2);
    bool showOnlyCurrentLanguage = m_filterText.isEmpty() && m_showOnlyCurrentLanguage;

    flag = flag
        && (showOnlyCurrentLanguage ? !lang.isEmpty()
                    && (QLocale().name().startsWith(lang)
                        || IMWorker::instance()->enabledLanguages().contains(lang))
                                    : true);
    if (!m_filterText.isEmpty()) {
        flag = flag
            && (name.contains(m_filterText, Qt::CaseInsensitive)
                || uniqueName.contains(m_filterText, Qt::CaseInsensitive)
                || langCode.contains(m_filterText, Qt::CaseInsensitive)
                || languageName(uniqueName, langCode).contains(m_filterText, Qt::CaseInsensitive));
    }
    return flag;
}

bool IMProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (left.data(RowTypeRole) == LanguageType) {
        int result = compareCategories(left, right);
        if (result < 0) {
            return true;
        } else if (result > 0) {
            return false;
        }
    } else {
        int result = compareItems(left, right);
        if (result < 0) {
            return true;
        } else if (result > 0) {
            return false;
        }
    }

    QString l = left.data(Qt::DisplayRole).toString();
    QString r = right.data(Qt::DisplayRole).toString();
    return QCollator().compare(l, r) < 0;
}

static std::tuple<bool, CONTAIN_CUR_LANG> checkCategories(QAbstractItemModel *model,
                                                          const QModelIndex &index)
{
    bool containsEnabled = false;
    auto containsCurrentLanguage = CONTAIN_CUR_LANG::NO;

    InputMethodEntryList entries =
        IMWorker::instance()->availableIMEntries().value(index.data(Qt::DisplayRole).toString());
    for (const InputMethodEntry &imEntry : entries) {
        if (imEntry.enabled()) {
            containsEnabled = true;
        }
        QString lang = imEntry.languageCode();
        if (lang.isEmpty()) {
            continue;
        }
        if (QLocale().name() == lang) {
            containsCurrentLanguage = CONTAIN_CUR_LANG::YES;
        }

        if (containsCurrentLanguage == CONTAIN_CUR_LANG::NO) {
            if (QLocale().name().startsWith(lang.left(2))) {
                containsCurrentLanguage = CONTAIN_CUR_LANG::DIFF_VARIANT;
            }
        }
    }

    return { containsEnabled, containsCurrentLanguage };
}

int IMProxyModel::compareCategories(const QModelIndex &left, const QModelIndex &right) const
{
    bool leftContainsEnabledIM = false;
    auto leftContainsCurrentLanguage = CONTAIN_CUR_LANG::NO;
    std::tie(leftContainsEnabledIM, leftContainsCurrentLanguage) =
        checkCategories(sourceModel(), left);

    bool rightContainsEnabledIM = false;
    auto rightContainsCurrentLanguage = CONTAIN_CUR_LANG::NO;
    std::tie(rightContainsEnabledIM, rightContainsCurrentLanguage) =
        checkCategories(sourceModel(), right);

    if (leftContainsEnabledIM != rightContainsEnabledIM) {
        return leftContainsEnabledIM ? -1 : 1;
    }

    if (leftContainsCurrentLanguage == rightContainsCurrentLanguage) {
        return 0;
    }

    if (leftContainsCurrentLanguage == CONTAIN_CUR_LANG::YES) {
        return -1;
    }

    if (rightContainsCurrentLanguage == CONTAIN_CUR_LANG::YES) {
        return 1;
    }

    if (leftContainsCurrentLanguage == CONTAIN_CUR_LANG::DIFF_VARIANT) {
        return -1;
    }

    if (rightContainsCurrentLanguage == CONTAIN_CUR_LANG::DIFF_VARIANT) {
        return 1;
    }

    return 0;
}

int IMProxyModel::compareItems(const QModelIndex &left, const QModelIndex &right) const
{
    bool leftEnabled = left.data(IMEnabledRole).toBool();
    bool rightEnabled = right.data(IMEnabledRole).toBool();

    if (leftEnabled == rightEnabled) {
        return 0;
    }

    if (leftEnabled) {
        return -1;
    }

    return 1;
}

class AvailItemDelegate : public DStyledItemDelegate
{
public:
    using DStyledItemDelegate::DStyledItemDelegate;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt(option);
        // 对选中项增加 checked 样式，删除选中样式，并增加鼠标悬停样式
        opt.features |= QStyleOptionViewItem::HasCheckIndicator;
        if (opt.state & QStyle::State_Selected) {
            opt.checkState = Qt::Checked;
            opt.state &= ~QStyle::State_Selected;
            opt.state |= QStyle::State_MouseOver;
        }

        DStyledItemDelegate::paint(painter, opt, index);

        const int useCount = IMWorker::instance()->enabledLanguageCount();
        if (Q_LIKELY(m_line) && useCount > 0 && (useCount - 1 == index.row())) {
            const DPalette &dp = DPaletteHelper::instance()->palette(m_line);
            const QColor &outlineColor = dp.frameBorder().color();
            QPoint start(opt.rect.left(), opt.rect.bottom() - (margins().bottom() / 2));
            painter->fillRect(QRect(start, QSize(opt.rect.width(), m_line->lineWidth())),
                              outlineColor);
        }
    }

    inline void setLineSplitter(QFrame *line) { m_line = line; }

private:
    QPointer<QFrame> m_line;
};

IMPage::IMPage(IMWorker *worker, QWidget *parent)
    : DAbstractDialog(parent)
    , m_worker(worker)
    , m_model(new QStandardItemModel(this))
    , m_childModel(new QStandardItemModel(this))
    , m_proxyModel(new IMProxyModel(this))
    , m_proxyChildModel(new IMProxyModel(this))
{
    setFixedSize(690, 620);

    auto *wrapperLayout = new QVBoxLayout(this);
    wrapperLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(wrapperLayout);

    DTitlebar *titleIcon = new DTitlebar();
    titleIcon->setFrameStyle(QFrame::NoFrame); // 无边框
    titleIcon->setBackgroundTransparent(true); // 透明
    titleIcon->setMenuVisible(false);
    titleIcon->setTitle(tr("Select your language and add input methods"));
    titleIcon->setIcon(QIcon(":/img/title_img.png"));
    wrapperLayout->addWidget(titleIcon);

    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(20, 20, 20, 10);
    mainLayout->setSpacing(10);
    wrapperLayout->addLayout(mainLayout);

    m_contentLayout = new QHBoxLayout();
    mainLayout->addLayout(m_contentLayout);

    auto *left = new DFrame(this);
    left->setFixedWidth(290);

    m_contentLayout->addWidget(left);

    m_leftLayout = new QVBoxLayout();
    m_leftLayout->setSpacing(10);
    m_leftLayout->setContentsMargins(10, 10, 10, 10);
    left->setLayout(m_leftLayout);

    m_searchEdit = new Dtk::Widget::DSearchEdit();
    m_leftLayout->addWidget(m_searchEdit);

    line = new DHorizontalLine(this);
    m_leftLayout->addWidget(line);

    m_availIMList = new DListView(this);
    auto availIMDelegate = new AvailItemDelegate(m_availIMList);
    availIMDelegate->setLineSplitter(line);
    m_availIMList->setItemDelegate(availIMDelegate);
    m_availIMList->setModel(m_proxyModel);
    m_proxyModel->setSourceModel(m_model);
    m_availIMList->setBackgroundType(DStyledItemDelegate::BackgroundType::RoundedBackground);
    m_availIMList->setFocusPolicy(Qt::NoFocus);
    m_availIMList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_availIMList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_leftLayout->addWidget(m_availIMList);

    m_rightLayout = new QVBoxLayout();
    m_rightLayout->setSpacing(10);
    m_contentLayout->addLayout(m_rightLayout);

    DFrame *rightListFrame = new DFrame(this);
    rightListFrame->setFixedWidth(350);
    QVBoxLayout *rightListFrameLayout = new QVBoxLayout();
    rightListFrame->setLayout(rightListFrameLayout);
    m_rightLayout->addWidget(rightListFrame);

    m_childIMList = new DListView(this);
    m_childIMList->setModel(m_proxyChildModel);
    m_proxyChildModel->setSourceModel(m_childModel);
    m_childIMList->setBackgroundType(DStyledItemDelegate::BackgroundType::RoundedBackground);
    m_childIMList->setFocusPolicy(Qt::NoFocus);
    m_childIMList->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    m_childIMList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightListFrameLayout->addWidget(m_childIMList);

    m_laSelector = new KeyboardWidget(this);
    m_laSelector->setFixedSize(350, 140);
    m_rightLayout->addWidget(m_laSelector);

    auto *findMoreLayout = new QHBoxLayout(this);
    mainLayout->addLayout(findMoreLayout);

    m_findMoreLabel = new DCommandLinkButton(tr("Find more in App Store"), this);
    m_findMoreLabel->setAccessibleName("Find more in App Store");
    findMoreLayout->addWidget(m_findMoreLabel);
    findMoreLayout->addStretch(1);

    mainLayout->addSpacing(9);

    m_buttonTuple = new ButtonTuple(ButtonTuple::Save, this);
    mainLayout->addWidget(m_buttonTuple);

    QPushButton *cancel = m_buttonTuple->leftButton();
    cancel->setText(tr("Cancel"));
    cancel->setObjectName("Cancel");
    QPushButton *ok = m_buttonTuple->rightButton();
    ok->setText(tr("Add"));
    ok->setEnabled(false);
    //  TODO:
    /* connect(m_config, &IMConfig::changed, this, &IMPage::changed); */

    connect(m_searchEdit,
            &Dtk::Widget::DSearchEdit::textChanged,
            qobject_cast<IMProxyModel *>(m_proxyModel),
            &IMProxyModel::setFilterText);
    connect(m_searchEdit,
            &Dtk::Widget::DSearchEdit::textChanged,
            qobject_cast<IMProxyModel *>(m_proxyChildModel),
            &IMProxyModel::setFilterText);

    connect(m_availIMList->model(), &QAbstractItemModel::layoutChanged, this, [this]() {
        m_availIMList->setCurrentIndex(m_availIMList->model()->index(0, 0));
    });
    connect(m_availIMList->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &IMPage::availIMCurrentChanged,
            Qt::QueuedConnection);
    connect(m_childIMList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &IMPage::childIMSelectionChanged);

    connect(m_findMoreLabel, &DCommandLinkButton::clicked, this, &IMPage::clickedFindMoreButton);

    connect(cancel, &QPushButton::clicked, this, &IMPage::clickedCloseButton);
    connect(ok, &QPushButton::clicked, this, &IMPage::clickedAddButton);
    connect(m_worker,
            &IMWorker::fetchAvailableIMEntriesFinished,
            this,
            [this](const QMap<QString, InputMethodEntryList> &availableIMEntries) {
                m_filteredIMEntryList = availableIMEntries;
                m_model->clear();
                for (auto &entry : availableIMEntries.keys()) {
                    DStandardItem *item = new DStandardItem;
                    item->setData(entry, Qt::DisplayRole);
                    item->setData(entry, Qt::ToolTipRole);
                    item->setData(availableIMEntries.value(entry).at(0).languageCode(),
                                  LanguageRole);
                    item->setData(LanguageType, RowTypeRole);
                    item->setData(
                        QVariant::fromValue(QPair<int, int>{ QPalette::Base, DPalette::NoType }),
                        Dtk::ViewItemBackgroundRole);
                    m_model->appendRow(item);
                }
                QTimer::singleShot(100, [this] {
                    m_availIMList->setCurrentIndex(m_availIMList->model()->index(0, 0));
                });
            });
    m_worker->fetchGroups();
    m_worker->fetchAvailableInputMethodEntries();
}

IMPage::~IMPage() { }

void IMPage::save()
{
    QItemSelectionModel *selections = m_childIMList->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();

    // TODO:
    /* m_config->addIMs(selected); */

    /* m_config->save(); */
}

void IMPage::load()
{
    // TODO:
    /* m_config->load(); */
}

void IMPage::defaults() { }

static std::tuple<QString, QString> getLayoutString(const QString &uniqName)
{
    QString layout = "";
    QString variant = "";
    if (uniqName.startsWith("keyboard-")) {
        // layout, name likes keyboard-xx-xxx, such as keyboard-cn-mon_trad_manchu
        auto layoutPos = uniqName.indexOf("-") + 1;
        auto secondDashPos = uniqName.indexOf("-", layoutPos);
        if (secondDashPos > 0) {
            int variantPos = secondDashPos + 1;
            layout = uniqName.mid(layoutPos, secondDashPos - layoutPos);
            variant = uniqName.mid(variantPos);
        } else {
            layout = uniqName.mid(layoutPos);
        }
    }
    return { layout, variant };
}

void IMPage::availIMCurrentChanged(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    QStandardItem *parentItem = m_model->itemFromIndex(m_proxyModel->mapToSource(index));
    if (!parentItem) {
        return;
    }

    m_childModel->clear();
    DStandardItem *defaultItem = nullptr;
    const QString key = parentItem->data(Qt::DisplayRole).toString();
    const InputMethodEntryList imEntries = m_filteredIMEntryList.value(key);
    for (const InputMethodEntry &imEntry : imEntries) {
        DStandardItem *item = new DStandardItem;
        item->setData(imEntry.name(), Qt::DisplayRole);
        item->setData(imEntry.name(), Qt::ToolTipRole);
        item->setData(IMType, RowTypeRole);
        item->setData(imEntry.uniqueName(), IMUniqueNameRole);
        item->setData(imEntry.languageCode(), LanguageRole);
        item->setData(imEntry.enabled(), IMEnabledRole);
        item->setData(imEntry.configurable(), IMConfigurableRole);
        item->setData(QVariant::fromValue(QPair<int, int>{ QPalette::Base, DPalette::NoType }),
                      Dtk::ViewItemBackgroundRole);
        item->setData(languageName(imEntry.uniqueName(), imEntry.languageCode()), LanguageNameRole);
        item->setEnabled(!imEntry.enabled());
        m_childModel->appendRow(item);
    }
    // 默认选择第一个未被启用的输入法，当不存在未被启用的输入法时，选择空 index（清空选择）
    for (int row = 0; row < m_childIMList->model()->rowCount(); ++row) {
        QModelIndex index = m_childIMList->model()->index(row, 0);
        if (index.isValid() && !index.data(IMEnabledRole).toBool()) {
            m_childIMList->setCurrentIndex(index);
            break;
        }
    }
}

void IMPage::childIMSelectionChanged()
{
    auto selected = m_childIMList->selectionModel()->selectedIndexes();

    m_buttonTuple->rightButton()->setEnabled(!selected.isEmpty());

    if (selected.isEmpty()) {
        m_laSelector->showNoLayout();
        return;
    }

    if (selected.count() > 1) {
        m_laSelector->showMulti();
        return;
    }

    QModelIndex i = selected.at(0);
    QString uniqueName = i.data(IMUniqueNameRole).toString();

    QString layout;
    QString variant;
    std::tie(layout, variant) = getLayoutString(uniqueName);
    m_laSelector->setKeyboardLayout(layout, variant);
}

void IMPage::clickedFindMoreButton()
{
    m_worker->launchStore();
}

void IMPage::clickedCloseButton()
{
    close();
    deleteLater();
}

void IMPage::clickedAddButton()
{
    save();

    close();
    deleteLater();
}

void IMPage::updateAvailableIMList()
{
    InputMethodEntryList imEntries = m_worker->allIMEntries();
    for (auto &entry : imEntries) {
        DStandardItem *item = new DStandardItem;
        item->setText(entry.name());
        m_model->appendRow(item);
    }
}

#include "impage.moc"

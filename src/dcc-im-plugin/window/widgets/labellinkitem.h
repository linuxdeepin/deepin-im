// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef LABELLINKITEM_H
#define LABELLINKITEM_H

#include "interface/namespace.h"

#include <dcommandlinkbutton.h>
#include <dtiplabel.h>
#include <dtkwidget_global.h>
#include <qlabel.h>
#include <qt6/QtCore/qtmetamacros.h>
#include <widgets/settingsitem.h>

DWIDGET_BEGIN_NAMESPACE
class DCommandLinkButton;
class DTipLabel;
DWIDGET_END_NAMESPACE

namespace dcc_im_plugin {
namespace widgets {

class LabelLinkItem : public DCC_NAMESPACE::SettingsItem
{
    Q_OBJECT
public:
    explicit LabelLinkItem(const QString &tip, const QString &link, QWidget *parent = nullptr);
    ~LabelLinkItem() override = default;

protected:
    void resizeEvent(QResizeEvent *event) override;

Q_SIGNALS:
    void linkButtonCLicked(bool clicked = false);

private:
    Dtk::Widget::DTipLabel *m_tipLabel = nullptr;
    Dtk::Widget::DCommandLinkButton *m_linkBtn = nullptr;
};

} // namespace widgets
} // namespace dcc_im_plugin
#endif // !DEBUG

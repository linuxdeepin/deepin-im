// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "labellinkitem.h"

#include <dcommandlinkbutton.h>
#include <dtiplabel.h>
#include <dtkwidget_global.h>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qt6/QtCore/qglobal.h>
#include <qwidget.h>

using namespace DCC_NAMESPACE;
DWIDGET_USE_NAMESPACE
using namespace dcc_im_plugin::widgets;

LabelLinkItem::LabelLinkItem(const QString &tip, const QString &link, QWidget *parent)
    : SettingsItem(parent)
    , m_tipLabel(new DTipLabel(tip))
    , m_linkBtn(new DCommandLinkButton(link))
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(m_tipLabel);
    hLayout->addWidget(m_linkBtn);
    hLayout->addStretch();
    connect(m_linkBtn, &QAbstractButton::clicked, this, &LabelLinkItem::linkButtonCLicked);
}

void LabelLinkItem::resizeEvent(QResizeEvent *event)
{
    if (m_tipLabel) {
        QFontMetrics fontMetrics(m_tipLabel->font());
        int fontSize = fontMetrics.horizontalAdvance(m_tipLabel->text());
        if (fontSize > m_tipLabel->width()) {
            m_tipLabel->setText(
                fontMetrics.elidedText(m_tipLabel->text(), Qt::ElideRight, m_tipLabel->width()));

            m_tipLabel->setToolTip(m_tipLabel->text());
        } else {
            m_tipLabel->setText(m_tipLabel->text());
            m_tipLabel->setToolTip("");
        }
    }
}

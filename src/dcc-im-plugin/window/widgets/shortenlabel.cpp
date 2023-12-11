// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#include "shortenlabel.h"

using namespace dcc_im_plugin::widgets;

ShortenLabel::ShortenLabel(const QString &text, QWidget *parent)
    : m_text(text)
    , QLabel(text, parent)
{
}

void ShortenLabel::setShortenText(const QString &text)
{
    m_text = text;
    setText(m_text);
    updateTitleSize();
}

QString ShortenLabel::text() const
{
    return m_text;
}

void ShortenLabel::resizeEvent(QResizeEvent *event)
{
    updateTitleSize();
    QLabel::resizeEvent(event);
}

void ShortenLabel::updateTitleSize()
{
    QFontMetrics fontMetrics(font());
    int fontSize = fontMetrics.width(m_text); // 获取之前设置的字符串的像素大小

    QString str = m_text;
    if (fontSize > width()) {
        str = fontMetrics.elidedText(m_text, Qt::ElideRight, width()); // 返回一个带有省略号的字符串
    }
    this->setText(str);
    show();
}

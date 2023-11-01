// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SHORTENLABEL_H
#define SHORTENLABEL_H

#include <QLabel>

namespace dcc_im_plugin {
namespace widgets {

class ShortenLabel : public QLabel
{
    Q_OBJECT
public:
    ShortenLabel(const QString &text = "", QWidget *parent = nullptr);
    void setShortenText(const QString &text);
    QString text() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateTitleSize();

private:
    QString m_text;
};

} // namespace widgets
} // namespace dcc_im_plugin
#endif // SHORTENLABEL_H

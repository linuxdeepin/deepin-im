// SPDX-FileCopyrightText: 2023 Deepin Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ADDIM_LAYOUTWIDGET_H
#define ADDIM_LAYOUTWIDGET_H

#include <DFrame>

struct xkb_context;

class QLabel;

enum class LayoutStatus;

namespace dcc_im_plugin {
namespace widgets {

class KeyboardWidget : public DTK_WIDGET_NAMESPACE::DFrame
{
    Q_OBJECT

public:
    KeyboardWidget(QWidget *parent = 0);
    ~KeyboardWidget();

    void setKeyboardLayout(const QString &layout, const QString &variant = QString());
    void showMulti();
    void showNoLayout();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    xkb_context *m_ctx;

    LayoutStatus m_status;
    std::string m_layout;
    std::string m_variant;

    QLabel *m_label;

    void paintLayout(QPainter &painter);
    void showNoLayoutLabel();
    void showMultiLayoutLabel();
};
} // namespace widgets
} // namespace dcc_im_plugin

#endif

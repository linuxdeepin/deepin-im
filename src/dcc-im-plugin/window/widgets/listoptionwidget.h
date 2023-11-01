// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LISTOPTIONWIDGET_H_
#define LISTOPTIONWIDGET_H_

#include "optionwidget.h"
#include "ui_listoptionwidget.h"

namespace dcc_im_plugin {
namespace widgets {
class ListOptionWidgetModel;

class ListOptionWidget : public OptionWidget, public Ui::ListOptionWidget
{
    Q_OBJECT
public:
    ListOptionWidget(const operation::ConfigOption &option, const QString &path, QWidget *parent);

    void readValueFrom(const QVariantMap &map) override;
    void writeValueTo(QVariantMap &map) override;
    void restoreToDefault() override;

    const auto &subOption() { return subOption_; }

private:
    void updateButton();
    ListOptionWidgetModel *model_;
    operation::ConfigOption subOption_;
    QVariantMap defaultValue_;
};
} // namespace widgets
} // namespace dcc_im_plugin

#endif // LISTOPTIONWIDGET_H_

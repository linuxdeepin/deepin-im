// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <QObject>

#include <QString>

namespace org {
namespace deepin {
namespace dim {

class Dim;
class InputMethodAddon;
class InputMethodEntry;

class InputState: public QObject
{
    Q_OBJECT

public:
    InputState(Dim *dim);

    inline const QString &currentIMAddon() const { return current_im_addon_; }

private Q_SLOTS:
    void handleImChanged();

private:
    Dim *dim_;
    QList<QString> ims_;
    QString current_im_addon_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H

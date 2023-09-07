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
class InputContext;
class InputMethodAddon;
class InputMethodEntry;

class InputState : public QObject
{
    Q_OBJECT

public:
    InputState(InputContext *ic);

    inline const QString &currentIMAddon() const { return currentImAddon_; }

    void switchIMAddon();

private:
    InputContext *ic_;
    QList<QString> ims_;
    int currentImIdx_;
    QString currentImAddon_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H

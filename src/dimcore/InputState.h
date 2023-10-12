// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "InputMethodEntry.h"

#include <QObject>
#include <QPair>

namespace org {
namespace deepin {
namespace dim {

typedef QPair<QString, QString> IMIndex;

class Dim;
class InputContext;
class InputMethodEntry;

class InputState : public QObject
{
    Q_OBJECT

public:
    explicit InputState(InputContext *ic);

    int currentIMIndex() const;
    void switchIMAddon();

private:
    IMIndex currentIMIndex_;
    InputContext *ic_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H

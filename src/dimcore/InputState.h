// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "InputMethodEntry.h"

#include <QMap>
#include <QObject>
#include <QString>

namespace org {
namespace deepin {
namespace dim {

class Dim;
class InputContext;
class InputMethodEntry;

class InputState : public QObject
{
    Q_OBJECT

public:
    explicit InputState(InputContext *ic);

    const InputMethodEntry &currentIMEntry() const;
    void switchIMAddon();

private:
    InputContext *ic_;
    QMap<QString, InputMethodEntry>::const_iterator imEntryIt_;
    QMap<QString, InputMethodEntry> imEntries_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H

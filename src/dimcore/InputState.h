// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "InputMethodEntry.h"

#include <QObject>

namespace org {
namespace deepin {
namespace dim {

typedef std::pair<std::string, std::string> IMKey;

class Dim;
class InputContext;
class InputMethodEntry;

class InputState : public QObject
{
    Q_OBJECT

public:
    explicit InputState(InputContext *ic);

    std::vector<InputMethodEntry>::const_iterator currentIMEntry() const;
    void switchIMAddon();

private:
    IMKey currentIMKey_;
    InputContext *ic_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H

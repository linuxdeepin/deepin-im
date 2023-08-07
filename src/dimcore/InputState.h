// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <QString>

namespace org {
namespace deepin {
namespace dim {

class Dim;
class InputMethodAddon;
class InputMethodEntry;

class InputState
{
public:
    InputState(Dim *dim);

    inline const QString &currentIM() const { return current_im_; }

private:
    Dim *dim_;
    QString current_im_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QString>

#include <memory>

class InputMethodEntryPrivate;

namespace org {
namespace deepin {
namespace dim {

class InputMethodEntry
{
public:
    InputMethodEntry(const QString &addon,
                     const QString &uniqueName,
                     const QString &name,
                     const QString &description,
                     const QString &label,
                     const QString &iconName);
    ~InputMethodEntry();

    const QString &addonName() const;
    const QString &uniqueName() const;
    const QString &name() const;
    const QString &description() const;
    const QString &label() const;
    const QString &iconName() const;

private:
    std::shared_ptr<InputMethodEntryPrivate> d;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHOD_H

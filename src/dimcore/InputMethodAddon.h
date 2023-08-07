// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODADDON_H
#define INPUTMETHODADDON_H

#include "Addon.h"
#include "InputMethodEntry.h"

#include <QList>

namespace org {
namespace deepin {
namespace dim {

class InputMethodAddon : public Addon
{
    Q_OBJECT

public:
    explicit InputMethodAddon(Dim *dim, const QString &key);
    virtual ~InputMethodAddon();

    virtual QList<InputMethodEntry> getInputMethods() = 0;
    virtual void initInputMethods() = 0;
    virtual bool keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) = 0;

Q_SIGNALS:
    void addonInitFinished(InputMethodAddon *imAddon);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHODADDON_H

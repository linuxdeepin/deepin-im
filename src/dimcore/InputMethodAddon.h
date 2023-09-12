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

class InputContextSetSurroundingTextEvent;

class InputMethodAddon : public Addon
{
    Q_OBJECT

public:
    explicit InputMethodAddon(Dim *dim, const QString &key, const QString &iconName);
    virtual ~InputMethodAddon();

    virtual QList<InputMethodEntry> getInputMethods() = 0;
    virtual void initInputMethods() = 0;
    virtual bool keyEvent(InputContextKeyEvent &keyEvent) = 0;
    virtual void setSurroundingText(InputContextSetSurroundingTextEvent &event) = 0;

    const QString &iconName() { return iconName_; }

Q_SIGNALS:
    void addonInitFinished(InputMethodAddon *imAddon);

private:
    const QString iconName_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHODADDON_H

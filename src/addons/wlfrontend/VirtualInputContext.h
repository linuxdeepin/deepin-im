// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALINPUTCONTEXT_H
#define VIRTUALINPUTCONTEXT_H

#include <dimcore/InputContext.h>

namespace org::deepin::dim {

class VirtualInputContextGlue;

class VirtualInputContext : public InputContext
{
public:
    VirtualInputContext(VirtualInputContextGlue *parentIC, Dim *dim);
    ~VirtualInputContext();

protected:
    void updatePreeditImpl(const QString &text, int32_t cursorBegin, int32_t cursorEnd) override;
    void commitStringImpl(const QString &text) override;
    void forwardKeyImpl(uint32_t keycode, bool pressed) override;

private:
    VirtualInputContextGlue *parentIC_;
};

} // namespace org::deepin::dim

#endif // !VIRTUALINPUTCONTEXT_H

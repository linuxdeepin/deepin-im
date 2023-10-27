// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROXYADDON_H
#define PROXYADDON_H

#include "InputMethodAddon.h"

#include <QList>

namespace org {
namespace deepin {
namespace dim {

class InputContext;
class InputContextCursorRectChangeEvent;

class ProxyAddon : public InputMethodAddon
{
    Q_OBJECT

public:
    explicit ProxyAddon(Dim *dim, const std::string &key, const QString &iconName);
    virtual ~ProxyAddon();

    virtual void focusIn(uint32_t id) = 0;
    virtual void focusOut(uint32_t id) = 0;
    virtual void destroyed(uint32_t id) = 0;
    virtual void createFcitxInputContext(InputContext *) = 0;
    virtual void cursorRectangleChangeEvent(InputContextCursorRectChangeEvent &event) = 0;
    virtual void setCurrentIM(const std::string &im) = 0;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // PROXYADDON_H

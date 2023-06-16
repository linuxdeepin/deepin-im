// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROXYADDON_H
#define PROXYADDON_H

#include "InputMethodAddon.h"
#include "InputMethodEntry.h"

#include <QList>

namespace org {
namespace deepin {
namespace dim {

class InputContext;

class ProxyAddon : public InputMethodAddon
{
    Q_OBJECT

public:
    explicit ProxyAddon(Dim *dim, const QString &key);
    virtual ~ProxyAddon();

    virtual void focusIn(uint32_t id) = 0;
    virtual void focusOut(uint32_t id) = 0;
    virtual void destroyed(uint32_t id) = 0;
    virtual void createFcitxInputContext(InputContext *) = 0;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // PROXYADDON_H

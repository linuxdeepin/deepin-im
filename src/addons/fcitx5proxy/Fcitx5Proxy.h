// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FCITX5PROXY_H
#define FCITX5PROXY_H

#include "Fcitx5InputContextIface.h"

#include <dimcore/ProxyAddon.h>

#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusPendingReply>

namespace org {
namespace deepin {
namespace dim {

class DBusProvider;

class Fcitx5Proxy : public ProxyAddon
{
public:
    Fcitx5Proxy(Dim *dim);
    virtual ~Fcitx5Proxy();

    void initInputMethods() override;
    QList<InputMethodEntry> getInputMethods() override;
    bool keyEvent(InputContextKeyEvent &keyEvent) override;
    void createFcitxInputContext(InputContext *ic) override;
    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;

    void addCapability(uint32_t id);

private:
    inline bool isICDBusInterfaceValid(uint32_t id)
    {
        return !icMap_.isEmpty() && icMap_.contains(id) && icMap_[id]->isValid();
    }

private:
    DBusProvider *dbusProvider_;
    bool available_;
    QMap<uint32_t, org::fcitx::Fcitx::InputContext1 *> icMap_;
    QList<InputMethodEntry> inputMethods_;

    void updateInputMethods();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !FCITX5PROXY_H

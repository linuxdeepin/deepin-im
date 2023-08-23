// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IBUSPROXY_H
#define IBUSPROXY_H

#include "IBUSInputContextIface.h"

#include <dimcore/ProxyAddon.h>
#include <ibus.h>

namespace org {
namespace deepin {
namespace dim {

class DimIBusProxy : public ProxyAddon
{
public:
    DimIBusProxy(Dim *dim);
    virtual ~DimIBusProxy();

    QList<InputMethodEntry> getInputMethods() override;
    void initInputMethods() override;
    bool keyEvent(InputContextKeyEvent &keyEvent) override;
    void createFcitxInputContext(InputContext *ic) override;
    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;
    bool isUseSyncMode();

    void updateInputMethods();

private:
    inline bool isICDBusInterfaceValid(uint32_t id)
    {
        return !iBusICInterfaceMap_.isEmpty() && iBusICInterfaceMap_.contains(id)
            && iBusICInterfaceMap_[id]->isValid();
    }

    inline bool isIBusICValid(uint32_t id)
    {
        return !iBusICMap_.isEmpty() && iBusICMap_.contains(id) && iBusICMap_[id] != nullptr;
    }

public Q_SLOTS:
    void init();
    void finalize();
    void clean();

private:
    bool init_;
    QList<InputMethodEntry> inputMethods_;
    QMap<uint32_t, org::freedesktop::IBus::InputContext *> iBusICInterfaceMap_;
    QMap<uint32_t, IBusInputContext *> iBusICMap_;
    IBusBus *bus_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // IBUSPROXY_H
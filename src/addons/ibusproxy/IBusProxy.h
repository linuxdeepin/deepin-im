// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IBUSPROXY_H
#define IBUSPROXY_H

#include "IBUSInputContextIface.h"
#include "IBUSPortIface.h"

#include <dimcore/ProxyAddon.h>

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
    bool useAsyncMode() override;
    void createFcitxInputContext(InputContext *ic) override;
    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;

private:
    QString getSocketPath();

    inline bool isICDBusInterfaceValid(uint32_t id)
    {
        return !iBusICMap_.isEmpty() && iBusICMap_.contains(id) && iBusICMap_[id]->isValid();
    }

public Q_SLOTS:
    void init();
    void connectToBus();
    void socketChanged(const QString &str);
    void busRegistered(const QString &str);
    void busUnregistered(const QString &str);

private:
    bool useSyncMode_;
    QDBusConnection dbusConn_;
    QFileSystemWatcher socketWatcher_;
    OrgFreedesktopIBusInputContextInterface *context_;
    OrgFreedesktopIBusPortalInterface *portalBus_;
    QTimer timer_;
    QDBusServiceWatcher *serviceWatcher_;
    QList<InputMethodEntry> inputMethods_;
    QMap<uint32_t, OrgFreedesktopIBusInputContextInterface *> iBusICMap_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // IBUSPROXY_H
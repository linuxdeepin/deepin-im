// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IBUSPROXY_H
#define IBUSPROXY_H

#include "IBUSInputContextIface.h"

#include <dimcore/ProxyAddon.h>
#include <memory>

class IBusEngineDesc;
class DimIBusInputContextPrivate;

namespace org {
namespace deepin {
namespace dim {

class DimIBusProxy : public ProxyAddon
{
public:
    explicit DimIBusProxy(Dim *dim);
    ~DimIBusProxy() override;

    const QList<InputMethodEntry> &getInputMethods() override;
    void initInputMethods() override;
    bool keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) override;
    void cursorRectangleChangeEvent(InputContextCursorRectChangeEvent &event) override;
    void updateSurroundingText(Event &event) override;
    void createFcitxInputContext(InputContext *ic) override;
    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;
    void setCurrentIM(const QString &im) override;

private:
    QList<IBusEngineDesc> listEngines();
    void initEngines();

    inline bool isICDBusInterfaceValid(uint32_t id)
    {
        return !iBusICMap_.isEmpty() && iBusICMap_.contains(id) && iBusICMap_[id]->isValid();
    }

public Q_SLOTS:
    void connectToBus();
    void socketChanged(const QString &str);
    void busRegistered(const QString &str);
    void busUnregistered(const QString &str);
    void globalEngineChanged(const QString &engineName);

private:
    DimIBusInputContextPrivate *d;
    bool useSyncMode_;
    QFileSystemWatcher socketWatcher_;
    QTimer timer_;
    QList<InputMethodEntry> inputMethods_;
    QMap<uint32_t, std::shared_ptr<OrgFreedesktopIBusInputContextInterface>> iBusICMap_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // IBUSPROXY_H
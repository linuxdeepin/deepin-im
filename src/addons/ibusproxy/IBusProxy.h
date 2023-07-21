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
    bool keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) override;
    void createFcitxInputContext(InputContext *ic) override;
    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;

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

    void updateInputMethods();

public Q_SLOTS:
    void init();
    void finalize();
    void clean();

private:
    QList<InputMethodEntry> inputMethods_;
    QMap<uint32_t, org::freedesktop::IBus::InputContext *> iBusICInterfaceMap_;
    QMap<uint32_t, IBusInputContext *> iBusICMap_;
    IBusBus *m_bus;
    QDBusServiceWatcher *m_watcher;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // IBUSPROXY_H
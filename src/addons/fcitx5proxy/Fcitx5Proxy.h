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

    QList<InputMethodEntry> getInputMethods() override;
    bool keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) override;
    void createFcitxInputContext(InputContext *ic) override;
    void focusIn(uint32_t id) override;
    void focusOut(uint32_t id) override;
    void destroyed(uint32_t id) override;

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

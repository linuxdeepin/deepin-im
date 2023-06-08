#ifndef FCITX5PROXY_H
#define FCITX5PROXY_H

#include <dimcore/ProxyAddon.h>

#include <QDBusObjectPath>
#include <QDBusPendingReply>
#include <QDBusInterface>

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
    void keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent) override;
    void createInputContext(uint32_t id, const QString &appName) override;
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
    QMap<uint32_t, QDBusInterface *> icMap_;
    QList<InputMethodEntry> inputMethods_;

    void updateInputMethods();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !FCITX5PROXY_H

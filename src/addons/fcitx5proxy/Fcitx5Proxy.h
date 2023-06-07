#ifndef FCITX5PROXY_H
#define FCITX5PROXY_H

#include <dimcore/InputMethodAddon.h>

#include <QDBusObjectPath>
#include <QDBusPendingReply>
#include <QDBusInterface>

namespace org {
namespace deepin {
namespace dim {

class DBusProvider;

class Fcitx5Proxy : public InputMethodAddon
{
public:
    Fcitx5Proxy(Dim *dim);
    virtual ~Fcitx5Proxy();

    QList<InputMethodEntry> getInputMethods() override;
    void keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent) override;

private:
    inline bool isICDBusInterfaceValid(const QString &appName)
    {
        return !icMap_.isEmpty() && icMap_.contains(appName) && icMap_[appName]->isValid();
    }

private Q_SLOTS:
    void createFcitxInputContext(const QString &app);

private:
    DBusProvider *dbusProvider_;
    bool available_;
    QMap<QString, QDBusInterface *> icMap_;
    QList<InputMethodEntry> inputMethods_;

    void updateInputMethods();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !FCITX5PROXY_H

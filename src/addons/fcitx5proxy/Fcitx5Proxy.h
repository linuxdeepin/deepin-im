#ifndef FCITX5PROXY_H
#define FCITX5PROXY_H

#include <dimcore/InputMethodAddon.h>

#include <QDBusObjectPath>
#include <QDBusPendingReply>

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

private Q_SLOTS:
    QDBusPendingReply<QDBusObjectPath, QByteArray> createFcitxInputContext(const QString &app);

private:
    DBusProvider *dbusProvider_;
    bool available_;

    QList<InputMethodEntry> inputMethods_;

    void updateInputMethods();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !FCITX5PROXY_H

#include "Fcitx5Proxy.h"

#include "DBusProvider.h"
#include "dimcore/Events.h"

#include <QGuiApplication>

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(Fcitx5Proxy);

Fcitx5Proxy::Fcitx5Proxy(Dim *dim)
    : InputMethodAddon(dim, "fcitx5proxy")
    , dbusProvider_(new DBusProvider(this))
    , available_(dbusProvider_->available())
{
    connect(dbusProvider_, &DBusProvider::availabilityChanged, this, [this](bool available) {
        if (available_ != available) {
            available_ = available;

            available_ ? updateInputMethods() : inputMethods_.clear();
        }
    });

    connect(qobject_cast<InputMethodAddon *>(this),
            &InputMethodAddon::createInputContext,
            this,
            &Fcitx5Proxy::createFcitxInputContext);
    connect(qobject_cast<InputMethodAddon *>(this),
            &InputMethodAddon::focusIn,
            this,
            [this](const QString &appName) {
                if (isICDBusInterfaceValid(appName)) {
                    icMap_[appName]->asyncCall("FocusIn");
                }
            });
    connect(qobject_cast<InputMethodAddon *>(this),
            &InputMethodAddon::focusOut,
            this,
            [this](const QString &appName) {
                if (isICDBusInterfaceValid(appName)) {
                    icMap_[appName]->asyncCall("FocusOut");
                }
            });
    connect(qobject_cast<InputMethodAddon *>(this),
            &InputMethodAddon::destroyed,
            this,
            [this](const QString &appName) {
                if (isICDBusInterfaceValid(appName)) {
                    icMap_[appName]->asyncCall("DestroyIC");
                }
            });

    updateInputMethods();
}

Fcitx5Proxy::~Fcitx5Proxy()
{
    icMap_.clear();
}

QList<InputMethodEntry> Fcitx5Proxy::getInputMethods()
{
    return inputMethods_;
}

void Fcitx5Proxy::createFcitxInputContext(const QString &app)
{
    FcitxQtStringKeyValueList list;
    FcitxQtStringKeyValue arg;

    arg.setKey("program");
    arg.setValue(app);

    FcitxQtStringKeyValue arg2;
    arg2.setKey("display");
    if (QGuiApplication::platformName() == QLatin1String("xcb")) {
        arg2.setValue("x11:");
    } else if (QGuiApplication::platformName().startsWith("wayland")) {
        arg2.setValue("wayland:");
    }
    list << arg2;

    auto result = dbusProvider_->imProxy()->CreateInputContext(list);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result, this);
    QObject::connect(watcher,
                     &QDBusPendingCallWatcher::finished,
                     this,
                     [&](QDBusPendingCallWatcher *watcher) {
                         watcher->deleteLater();

                         QDBusPendingReply<QDBusObjectPath, QByteArray> reply = *watcher;
                         if (reply.isError()) {
                             qDebug()
                                 << "create fcitx input context error:" << reply.error().message();
                             return;
                         }

                         QDBusInterface *icIface =
                             new QDBusInterface("org.fcitx.Fcitx5",
                                                reply.value().path(),
                                                "org.fcitx.Fcitx.InputContext1",
                                                QDBusConnection::sessionBus(),
                                                this);
                         icMap_[app] = icIface;
                     });
}

void Fcitx5Proxy::keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent)
{
    Q_UNUSED(entry);
    // TODO: wait to inputcontext appname
    auto appName = QString();
    if (isICDBusInterfaceValid(appName)) {
        icMap_[appName]->asyncCall("ProcessKeyEvent ",
                                   keyEvent.keyValue(),
                                   keyEvent.keycode(),
                                   keyEvent.state(),
                                   keyEvent.isRelease(),
                                   keyEvent.time());
    }
}

void Fcitx5Proxy::updateInputMethods()
{
    auto call = dbusProvider_->controller()->AvailableInputMethods();
    auto watcher = new QDBusPendingCallWatcher(call, this);
    connect(
        watcher,
        &QDBusPendingCallWatcher::finished,
        this,
        [this](QDBusPendingCallWatcher *watcher) {
            QDBusPendingReply<FcitxQtInputMethodEntryList> ims = *watcher;
            watcher->deleteLater();

            QList<InputMethodEntry> inputMethods;
            for (auto &im : ims.value()) {
                inputMethods.append(
                    { key(), im.uniqueName(), im.name(), im.nativeName(), im.label(), im.icon() });
            }

            inputMethods_.swap(inputMethods);
        });
}

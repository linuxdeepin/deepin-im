#include "Fcitx5Proxy.h"

#include "DBusProvider.h"

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

    updateInputMethods();
}

Fcitx5Proxy::~Fcitx5Proxy() { }

QList<InputMethodEntry> Fcitx5Proxy::getInputMethods()
{
    return inputMethods_;
}

QDBusPendingReply<QDBusObjectPath, QByteArray>
Fcitx5Proxy::createFcitxInputContext(const QString &app)
{
    FcitxQtStringKeyValueList list;
    FcitxQtStringKeyValue arg;

    arg.setKey("program");
    arg.setValue(app);

    FcitxQtStringKeyValue arg2;
    arg2.setKey("display");
    arg2.setValue("x11:");
    list << arg2;

    auto result = dbusProvider_->imProxy()->CreateInputContext(list);
    if (!result.isError()) {
        qWarning() << "failed to create fcitx input context";
        return QDBusPendingReply{};
    }

    return result;
}

void Fcitx5Proxy::keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent)
{
    Q_UNUSED(entry);
    Q_UNUSED(keyEvent);
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

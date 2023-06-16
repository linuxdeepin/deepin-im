#include "Fcitx5Proxy.h"

#include "DBusProvider.h"
#include "dimcore/Events.h"
#include "dimcore/InputContext.h"

#include <QGuiApplication>

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(Fcitx5Proxy);

Fcitx5Proxy::Fcitx5Proxy(Dim *dim)
    : ProxyAddon(dim, "fcitx5proxy")
    , dbusProvider_(new DBusProvider(this))
    , available_(dbusProvider_->available())
{
    connect(dbusProvider_, &DBusProvider::availabilityChanged, this, [this](bool available) {
        if (available_ != available) {
            available_ = available;

            available_ ? updateInputMethods() : inputMethods_.clear();
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

void Fcitx5Proxy::createFcitxInputContext(InputContext *ic)
{
    if (!ic) {
        return;
    }

    FcitxQtInputContextProxy *icProxy = new FcitxQtInputContextProxy(dbusProvider_->watch(), ic);

    if (!icProxy) {
        qDebug() << "failed to create FcitxQtInputContextProxy";
        return;
    }

    connect(icProxy, &FcitxQtInputContextProxy::inputContextCreated, this, [=] {
        if (!icProxy->isValid()) {
            qDebug() << "invalid input context proxy";
            return;
        }

        icMap_[ic->id()] = icProxy;
    });
}

void Fcitx5Proxy::focusIn(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->focusIn();
    }
}

void Fcitx5Proxy::focusOut(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->focusOut();
    }
}

void Fcitx5Proxy::destroyed(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->destroyed();
    }
}

void Fcitx5Proxy::keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent)
{
    Q_UNUSED(entry);
    const auto id = keyEvent.ic()->id();
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->processKeyEvent(keyEvent.keyValue(),
                                    keyEvent.keycode(),
                                    keyEvent.state(),
                                    keyEvent.isRelease(),
                                    keyEvent.time());
    }

    // TODO: handle commitString
    // TODO: handle forwardkey
    // TODO: handle formatpreedit
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

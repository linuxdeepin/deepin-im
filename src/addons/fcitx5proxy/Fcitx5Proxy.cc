#include "Fcitx5Proxy.h"

#include "DBusProvider.h"

DIM_ADDON_FACTORY(Fcitx5Proxy);

Fcitx5Proxy::Fcitx5Proxy(Dim *dim)
    : InputMethodAddon(dim)
    , dbusProvider_(new DBusProvider(this))
    , available_(dbusProvider_->available()) {
    connect(dbusProvider_, &DBusProvider::availabilityChanged, this, [this](bool available) {
        if (available_ != available) {
            available_ = available;

            if (available_) {
                updateInputMethods();
            } else {
                inputMethods_.clear();
            }
        }
    });

    updateInputMethods();
}

Fcitx5Proxy::~Fcitx5Proxy() {
}

QList<InputMethodEntry> Fcitx5Proxy::getInputMethods() {
    QList<InputMethodEntry> ret;
    return ret;
}

void Fcitx5Proxy::updateInputMethods() {
    auto call = dbusProvider_->controller()->AvailableInputMethods();
    auto watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<FcitxQtInputMethodEntryList> ims = *watcher;
        watcher->deleteLater();

        QList<InputMethodEntry> inputMethods;
        for (auto &im : ims.value()) {
            inputMethods.append(InputMethodEntry(im.uniqueName(), im.name(), im.nativeName(), im.label(), im.icon()));
        }

        inputMethods_.swap(inputMethods);
    });
}

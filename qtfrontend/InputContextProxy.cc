#include "InputContextProxy.h"

static const QString DIM_SERVICE = "org.deepin.dim";
static const QString DIM_IM_PATH = "/org/freedesktop/portal/inputmethod";
static const QString DIM_IC_PATH = "/org/freedesktop/portal/inputmethod/%d";

InputContextProxy::InputContextProxy(QObject *parent)
    : QObject(parent)
    , bus_(QDBusConnection::connectToBus(QDBusConnection::BusType::SessionBus, "dim"))
    , watcher_(DIM_SERVICE, bus_, QDBusServiceWatcher::WatchModeFlag::WatchForOwnerChange, this)
    , im_(new org::deepin::dim::portal::inputmethod(DIM_SERVICE, DIM_IM_PATH, QDBusConnection::sessionBus(), this))
    , ic_(nullptr) {
    connect(&watcher_, &QDBusServiceWatcher::serviceOwnerChanged, this, &InputContextProxy::serviceAvailableChanged);
    checkServiceAndCreateIC();
}

void InputContextProxy::checkServiceAndCreateIC() {
    if (!bus_.interface()->isServiceRegistered(DIM_SERVICE)) {
        return;
    }

    if (ic_) {
        delete ic_;
        ic_ = nullptr;
    }

    QDBusPendingReply<QDBusObjectPath> penddingReply = im_->CreateInputContext();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(penddingReply, this);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this, &InputContextProxy::createIcFinished);
}

void InputContextProxy::serviceAvailableChanged() {
    qDebug() << "onServiceOwnerChanged";
    QTimer::singleShot(100, this, &InputContextProxy::checkServiceAndCreateIC);
}

void InputContextProxy::createIcFinished(QDBusPendingCallWatcher *watcher) {
    watcher->deleteLater();

    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (reply.isError()) {
        qDebug() << "CreateInputContext error:" << reply.error().message();
        return;
    }

    QDBusObjectPath path = reply.value();
    ic_ = new org::deepin::dim::portal::inputcontext(DIM_SERVICE, path.path(), bus_, this);
}

void InputContextProxy::focusIn() {
    if (!ic_) {
        return;
    }

    ic_->FocusIn();
}

void InputContextProxy::focusOut() {
    if (!ic_) {
        return;
    }

    ic_->FocusOut();
}

void InputContextProxy::processKeyEvent(uint keyval, uint keycode, uint state, bool isRelease, uint time) {
    if (!ic_) {
        return;
    }

    ic_->ProcessKeyEvent(keyval, keycode, state, isRelease, time);
}

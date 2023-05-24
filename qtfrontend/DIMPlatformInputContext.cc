#include "DIMPlatformInputContext.h"

#include <QEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QDBusConnection>
#include <QDebug>

static const QString DIM_SERVICE = "org.deepin.dim.portal";
static const QString DIM_IM_PATH = "/org/deepin/dim/portal/inputmethod";
static const QString DIM_IC_PATH = "/org/deepin/dim/portal/inputcontext/%d";

DIMPlatformInputContext::DIMPlatformInputContext()
    : QPlatformInputContext()
    , im_(new org::deepin::dim::portal::inputmethod(DIM_SERVICE, DIM_IM_PATH, QDBusConnection::sessionBus(), this))
    , ic_(nullptr)
    , focusObject_(nullptr) {
}

bool DIMPlatformInputContext::isValid() const {
    return true;
}

void DIMPlatformInputContext::setFocusObject(QObject *object) {
    if (focusObject_) {
        focusObject_->removeEventFilter(this);
    }

    if (!object) {
        return;
    }

    QInputMethodQueryEvent qe(Qt::ImQueryInput | Qt::ImEnabled);
    QGuiApplication::sendEvent(object, &qe);

    if (!qe.value(Qt::ImEnabled).toBool()) {
        return;
    }

    focusObject_ = object;

    QDBusPendingReply<QDBusObjectPath> penddingReply = im_->CreateInputContext();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(penddingReply, this);
    QObject::connect(watcher,
                     SIGNAL(finished(QDBusPendingCallWatcher *)),
                     this,
                     SLOT(createIcFinished(QDBusPendingCallWatcher *)));
}

void DIMPlatformInputContext::showInputPanel() {
    qDebug() << "showInputPanel";
}

void DIMPlatformInputContext::hideInputPanel() {
    qDebug() << "hideInputPanel";
}

bool DIMPlatformInputContext::isInputPanelVisible() const {
    qDebug() << "isInputPanelVisible";
    return true;
}

bool DIMPlatformInputContext::eventFilter(QObject *object, QEvent *event) {
    if (object != focusObject_) {
        return false;
    }

    if (event->type() != QEvent::KeyPress || event->type() != QEvent::KeyRelease) {
        return false;
    }

    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() != Qt::Key_Return) {
        return false;
    }

    uint keyval = keyEvent->nativeVirtualKey();
    uint keycode = keyEvent->nativeScanCode();
    uint state = keyEvent->nativeModifiers();
    bool isRelease = keyEvent->type() == QEvent::KeyRelease;
    uint time = keyEvent->timestamp();
    ic_->ProcessKeyEvent(keyval, keycode, state, isRelease, time);

    return true;
}

void DIMPlatformInputContext::createIcFinished(QDBusPendingCallWatcher *call) {
    QDBusPendingReply<QDBusObjectPath> reply = call->reply();
    if (reply.isError()) {
        // todo: error
        return;
    }

    QDBusObjectPath path = reply.value();
    ic_ = new org::deepin::dim::portal::inputcontext(DIM_SERVICE, path.path(), QDBusConnection::sessionBus(), this);
    focusObject_->installEventFilter(this);
}

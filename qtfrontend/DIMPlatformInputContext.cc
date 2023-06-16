#include "DIMPlatformInputContext.h"

#include "InputContextProxy.h"

#include <QDBusConnection>
#include <QDebug>
#include <QEvent>
#include <QGuiApplication>
#include <QKeyEvent>

static const QString DIM_SERVICE = "org.deepin.dim";
static const QString DIM_IM_PATH = "/org/freedesktop/portal/inputmethod";

DIMPlatformInputContext::DIMPlatformInputContext()
    : QPlatformInputContext()
    , proxy_(new InputContextProxy(this))
    , focusObject_(nullptr)
{
    connect(proxy_,
            &InputContextProxy::preedit,
            this,
            &DIMPlatformInputContext::preedit);
    connect(proxy_, &InputContextProxy::commitString, this, &DIMPlatformInputContext::commitString);
}

bool DIMPlatformInputContext::isValid() const
{
    return true;
}

void DIMPlatformInputContext::setFocusObject(QObject *object)
{
    qDebug() << "setFocusObject";
    if (focusObject_ == object) {
        return;
    }

    if (focusObject_) {
        proxy_->focusOut();
        focusObject_->removeEventFilter(this);
        focusObject_ = nullptr;
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
    focusObject_->installEventFilter(this);
    proxy_->focusIn();
}

void DIMPlatformInputContext::showInputPanel()
{
    qDebug() << "showInputPanel";
}

void DIMPlatformInputContext::hideInputPanel()
{
    qDebug() << "hideInputPanel";
}

bool DIMPlatformInputContext::isInputPanelVisible() const
{
    qDebug() << "isInputPanelVisible";
    return true;
}

bool DIMPlatformInputContext::eventFilter(QObject *object, QEvent *event)
{
    if (object != focusObject_) {
        return false;
    }

    if (event->type() != QEvent::KeyPress && event->type() != QEvent::KeyRelease) {
        return false;
    }

    if (!proxy_->available()) {
        return false;
    }

    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

    uint keyval = keyEvent->nativeVirtualKey();
    uint keycode = keyEvent->nativeScanCode();
    uint state = keyEvent->nativeModifiers();
    bool isRelease = keyEvent->type() == QEvent::KeyRelease;
    uint time = keyEvent->timestamp();
    proxy_->processKeyEvent(keyval, keycode, state, isRelease, time);

    return true;
}

void DIMPlatformInputContext::preedit(const QList<QString> &preedit) {
    QInputMethodEvent qe(preedit.join(""), {});
    QGuiApplication::sendEvent(focusObject_, &qe);
}

void DIMPlatformInputContext::commitString(const QString &text) {
    QInputMethodEvent qe;
    qe.setCommitString(text);
    QGuiApplication::sendEvent(focusObject_, &qe);
}
#include "DIMPlatformInputContext.h"

#include <QEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QDebug>

bool DIMPlatformInputContext::isValid() const {
    return true;
}

void DIMPlatformInputContext::setFocusObject(QObject *object) {
    if (focusObject_) {
        focusObject_->removeEventFilter(this);
    }

    if (object) {
        QInputMethodQueryEvent qe(Qt::ImQueryInput | Qt::ImEnabled);
        QGuiApplication::sendEvent(object, &qe);

        if (!qe.value(Qt::ImEnabled).toBool()) {
            return;
        }

        focusObject_ = object;
        focusObject_->installEventFilter(this);
    }
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

    if (event->type() != QEvent::KeyPress) {
        return false;
    }

    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() != Qt::Key_Return) {
        return false;
    }

    QInputMethodEvent e("dddd", {});
    // e.setCommitString("dddd");
    QGuiApplication::sendEvent(object, &e);

    return true;
}

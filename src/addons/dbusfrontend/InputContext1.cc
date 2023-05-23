#include "InputContext1.h"

#include <QDBusConnection>

#include "InputcontextAdaptor.h"
#include "dimcore/Events.h"

static const QString inputcontextPath = "/org/freedesktop/portal/inputmethod/%1";

InputContext1::InputContext1(QObject *parent)
    : InputContext(parent)
    , adaptor_(new InputcontextAdaptor(this))
    , path_(inputcontextPath.arg(id())) {
    QDBusConnection::sessionBus().registerObject(path_, this);
}

InputContext1::~InputContext1() {
}

void InputContext1::FocusIn() {
}

void InputContext1::FocusOut() {
}

void InputContext1::Destroy() {
    deleteLater();
}

void InputContext1::ProcessKeyEvent(uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time) {
    KeyEvent e(keyval, keycode, state, isRelease, time);
    keyEvent(e);
}

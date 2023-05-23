#include "InputContext.h"

#include "Dim.h"

InputContext::InputContext(Dim *dim, QObject *parent)
    : QObject(parent)
    , ObjectId()
    , dim_(dim) {
}

void InputContext::destroy() {
    deleteLater();
}

void InputContext::focusIn() {
    emit focused();
}

void InputContext::focusOut() {
    emit unFocused();
}

void InputContext::keyEvent(KeyEvent &event) {
    dim_->postEvent(event);
}

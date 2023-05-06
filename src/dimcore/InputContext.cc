#include "InputContext.h"

InputContext::InputContext(QObject *parent)
    : QObject(parent)
    , ObjectId() {
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

#include "InputContext.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputContext::InputContext(Dim *dim, QObject *parent)
    : QObject(parent)
    , ObjectId()
    , dim_(dim)
    , inputState_(dim)
{
    dim_->inputContextCreated(this);
}

void InputContext::destroy()
{
    deleteLater();
}

void InputContext::focusIn()
{
    emit focused();
}

void InputContext::focusOut()
{
    emit unFocused();
}

void InputContext::keyEvent(KeyEvent &event)
{
    dim_->postEvent(event);
}

const InputState &InputContext::inputState() const
{
    return inputState_;
}

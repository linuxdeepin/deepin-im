#include "InputContext.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputContext::InputContext(Dim *dim, QObject *parent)
    : QObject(parent)
    , ObjectId()
    , dim_(dim)
    , inputState_(dim)
{
    Event e(EventType::InputContextCreated, this);
    dim_->postEvent(e);
}

void InputContext::destroy()
{
    deleteLater();

    Event e(EventType::InputContextDestroyed, this);
    dim_->postEvent(e);
}

void InputContext::focusIn()
{
    Event e(EventType::InputContextFocused, this);
    dim_->postEvent(e);
}

void InputContext::focusOut()
{
    Event e(EventType::InputContextUnfocused, this);
    dim_->postEvent(e);
}

void InputContext::keyEvent(InputContextKeyEvent &event)
{
    dim_->postEvent(event);
}

const InputState &InputContext::inputState() const
{
    return inputState_;
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputContext.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputContext::InputContext(Dim *dim, QObject *parent)
    : QObject(parent)
    , ObjectId()
    , dim_(dim)
    , inputState_(this)
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
    hasFocus_ = true;

    Event e(EventType::InputContextFocused, this);
    dim_->postEvent(e);
}

void InputContext::focusOut()
{
    hasFocus_ = false;

    Event e(EventType::InputContextUnfocused, this);
    dim_->postEvent(e);
}

bool InputContext::hasFocus() const
{
    return hasFocus_;
}

bool InputContext::keyEvent(InputContextKeyEvent &event)
{
    return dim_->postEvent(event);
}

void InputContext::updatePreedit(const QString &text, int32_t cursorBegin, int32_t cursorEnd)
{
    updatePreeditImpl(text, cursorBegin, cursorEnd);
}

void InputContext::commitString(const QString &text)
{
    commitStringImpl(text);
}

void InputContext::forwardKey(uint32_t keycode, bool pressed)
{
    forwardKeyImpl(keycode, pressed);
}

SurroundingText &InputContext::surroundingText()
{
    return surroundingText_;
}

void InputContext::updateSurroundingText()
{
    Event event(EventType::InputContextUpdateSurroundingText, this);
    dim_->postEvent(event);
}

InputState &InputContext::inputState()
{
    return inputState_;
}

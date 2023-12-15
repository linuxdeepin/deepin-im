// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodV1.h"

#include "Output.h"
#include "Server.h"
#include "inputmethodv1/InputMethodGrabV1.h"
#include "inputmethodv1/ZwpInputMethodContextV1.h"
#include "inputmethodv1/ZwpInputPanelV1.h"

WL_ADDONS_BASE_USE_NAMESPACE

InputMethodV1::InputMethodV1(Server *server)
    : server_(server)
{
    auto display = server->display();
    init(display);

    inputPanelV1_.reset(new ZwpInputPanelV1(this));
    inputPanelV1_->init(display);
}

InputMethodV1::~InputMethodV1() { }

void InputMethodV1::sendActivate()
{
    inputMethodContext_.reset(new ZwpInputMethodContextV1(this));
    if (!getResource()) {
        return;
    }

    inputMethodContext_->add(getResource()->client(), 0);

    if (!isInputMethodContextResourceValid()) {
        return;
    }

    zwp_input_method_v1_send_activate(getResource()->handle,
                                      inputMethodContext_->getResource()->handle);
}

bool InputMethodV1::isInputMethodContextResourceValid() const
{
    return inputMethodContext_ && inputMethodContext_->getResource();
}

void InputMethodV1::sendDeactivate()
{
    if (!getResource() || !isInputMethodContextResourceValid()) {
        return;
    }

    zwp_input_method_v1_send_deactivate(getResource()->handle,
                                        inputMethodContext_->getResource()->handle);
    inputMethodContext_->send_reset(inputMethodContext_->getResource()->handle);
}

void InputMethodV1::sendContentType(uint32_t hint, uint32_t purpose)
{
    if (!isInputMethodContextResourceValid()) {
        return;
    }

    inputMethodContext_->send_content_type(inputMethodContext_->getResource()->handle,
                                           hint,
                                           purpose);
}

void InputMethodV1::sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor)
{
    if (!isInputMethodContextResourceValid()) {
        return;
    }

    inputMethodContext_->send_surrounding_text(inputMethodContext_->getResource()->handle,
                                               text,
                                               cursor,
                                               anchor);
}

void InputMethodV1::setCursorRectangle(int x, int y, int width, int height) { }

void InputMethodV1::sendKey(uint32_t keycode, bool isRelease)
{
    if (!inputMethodContext_ || !inputMethodContext_->getKeyboardGrab()) {
        return;
    }

    uint32_t ts = getTimestamp();

    inputMethodContext_->getKeyboardGrab()->sendKey(0,
                                                    ts,
                                                    keycode,
                                                    isRelease ? WL_KEYBOARD_KEY_STATE_RELEASED
                                                              : WL_KEYBOARD_KEY_STATE_PRESSED);
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodContextV1.h"

#include "Output.h"
#include "Server.h"

WL_ADDONS_BASE_USE_NAMESPACE

InputMethodContextV1::InputMethodContextV1(Server *server, ZwpInputMethodV1 *input_method)
    : server_(server)
    , input_method_(input_method)
{
}

InputMethodContextV1::~InputMethodContextV1() { }

void InputMethodContextV1::sendActivate()
{
}

void InputMethodContextV1::sendDeactivate()
{
}

void InputMethodContextV1::sendContentType(uint32_t hint, uint32_t purpose)
{
}

void InputMethodContextV1::sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor)
{
}

void InputMethodContextV1::sendDone()
{
}

void InputMethodContextV1::setCursorRectangle(int x, int y, int width, int height)
{

}

void InputMethodContextV1::sendKey(uint32_t keycode, bool isRelease)
{

}

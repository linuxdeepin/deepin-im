// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodV1.h"

#include "Output.h"
#include "Server.h"

WL_ADDONS_BASE_USE_NAMESPACE

InputMethodV1::InputMethodV1(Server *server, ZwpInputMethodV1 *input_method)
    : server_(server)
    , input_method_(input_method)
{
}

InputMethodV1::~InputMethodV1() { }

void InputMethodV1::sendActivate()
{
}

void InputMethodV1::sendDeactivate()
{
}

void InputMethodV1::sendContentType(uint32_t hint, uint32_t purpose)
{
}

void InputMethodV1::sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor)
{
}

void InputMethodV1::sendDone()
{
}

void InputMethodV1::setCursorRectangle(int x, int y, int width, int height)
{

}

void InputMethodV1::sendKey(uint32_t keycode, bool isRelease)
{

}

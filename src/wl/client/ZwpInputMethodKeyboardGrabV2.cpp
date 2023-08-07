// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodKeyboardGrabV2.h"

using namespace wl::client;

ZwpInputMethodKeyboardGrabV2::ZwpInputMethodKeyboardGrabV2(zwp_input_method_keyboard_grab_v2 *val)
    : Type(val)
{
}

ZwpInputMethodKeyboardGrabV2::~ZwpInputMethodKeyboardGrabV2()
{
    zwp_input_method_keyboard_grab_v2_destroy(get());
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodV2.h"

#include "ZwpInputMethodKeyboardGrabV2.h"

using namespace wl::client;

ZwpInputMethodV2::ZwpInputMethodV2(zwp_input_method_v2 *val)
    : Type(val)
{
}

ZwpInputMethodV2::~ZwpInputMethodV2()
{
    zwp_input_method_v2_destroy(get());
}

std::shared_ptr<ZwpInputMethodKeyboardGrabV2> ZwpInputMethodV2::grabKeyboard()
{
    return std::make_shared<ZwpInputMethodKeyboardGrabV2>(zwp_input_method_v2_grab_keyboard(get()));
}

void ZwpInputMethodV2::commit_string(const char *text)
{
    zwp_input_method_v2_commit_string(get(), text);
}

void ZwpInputMethodV2::set_preedit_string(const char *text, int32_t cursorBegin, int32_t cursorEnd)
{
    zwp_input_method_v2_set_preedit_string(get(), text, cursorBegin, cursorEnd);
}

void ZwpInputMethodV2::commit(uint32_t serial)
{
    zwp_input_method_v2_commit(get(), serial);
}

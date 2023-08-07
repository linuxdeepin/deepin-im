// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpTextInputV3.h"

using namespace wl::client;

ZwpTextInputV3::ZwpTextInputV3(zwp_text_input_v3 *val)
    : Type(val)
{
}

ZwpTextInputV3::~ZwpTextInputV3()
{
    zwp_text_input_v3_destroy(get());
}

void ZwpTextInputV3::enable()
{
    zwp_text_input_v3_enable(get());
}

void ZwpTextInputV3::disable()
{
    zwp_text_input_v3_disable(get());
}

void ZwpTextInputV3::setSurroundingText(const char *text, int32_t cursor, int32_t anchor)
{
    zwp_text_input_v3_set_surrounding_text(get(), text, cursor, anchor);
}

void ZwpTextInputV3::setTextChangeCause(uint32_t cause)
{
    zwp_text_input_v3_set_text_change_cause(get(), cause);
}

void ZwpTextInputV3::setContentType(uint32_t hint, uint32_t purpose)
{
    zwp_text_input_v3_set_content_type(get(), hint, purpose);
}

void ZwpTextInputV3::setCursorRectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
    zwp_text_input_v3_set_cursor_rectangle(get(), x, y, width, height);
}

void ZwpTextInputV3::commit()
{
    zwp_text_input_v3_commit(get());
}

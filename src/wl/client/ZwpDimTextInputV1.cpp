// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpDimTextInputV1.h"

using namespace wl::client;

ZwpDimTextInputV1::ZwpDimTextInputV1(zwp_dim_text_input_v1 *val)
    : Type(val)
{
}

ZwpDimTextInputV1::~ZwpDimTextInputV1()
{
    zwp_dim_text_input_v1_destroy(get());
}

void ZwpDimTextInputV1::enable()
{
    zwp_dim_text_input_v1_enable(get());
}

void ZwpDimTextInputV1::disable()
{
    zwp_dim_text_input_v1_disable(get());
}

void ZwpDimTextInputV1::setSurroundingText(const char *text, int32_t cursor, int32_t anchor)
{
    zwp_dim_text_input_v1_set_surrounding_text(get(), text, cursor, anchor);
}

void ZwpDimTextInputV1::setTextChangeCause(uint32_t cause)
{
    zwp_dim_text_input_v1_set_text_change_cause(get(), cause);
}

void ZwpDimTextInputV1::setContentType(uint32_t hint, uint32_t purpose)
{
    zwp_dim_text_input_v1_set_content_type(get(), hint, purpose);
}

void ZwpDimTextInputV1::setCursorRectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
    zwp_dim_text_input_v1_set_cursor_rectangle(get(), x, y, width, height);
}

void ZwpDimTextInputV1::commit()
{
    zwp_dim_text_input_v1_commit(get());
}

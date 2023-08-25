// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpDimTextInputV1.h"

using namespace wl::client;

const zwp_dim_text_input_v1_listener ZwpDimTextInputV1::listener_ = {
    ListenerCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_modifiers_map>::func,
    ListenerCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_preedit_string>::func,
    ListenerCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_commit_string>::func,
    ListenerCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_delete_surrounding_text>::func,
    ListenerCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_done>::func,
    ListenerCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_keysym>::func,
};

ZwpDimTextInputV1::ZwpDimTextInputV1(zwp_dim_text_input_v1 *val)
    : Type(val)
{
    zwp_dim_text_input_v1_add_listener(get(), &listener_, this);
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

void ZwpDimTextInputV1::set_surrounding_text(const char *text, int32_t cursor, int32_t anchor)
{
    zwp_dim_text_input_v1_set_surrounding_text(get(), text, cursor, anchor);
}

void ZwpDimTextInputV1::set_text_change_cause(uint32_t cause)
{
    zwp_dim_text_input_v1_set_text_change_cause(get(), cause);
}

void ZwpDimTextInputV1::set_content_type(uint32_t hint, uint32_t purpose)
{
    zwp_dim_text_input_v1_set_content_type(get(), hint, purpose);
}

void ZwpDimTextInputV1::set_cursor_rectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
    zwp_dim_text_input_v1_set_cursor_rectangle(get(), x, y, width, height);
}

void ZwpDimTextInputV1::commit()
{
    zwp_dim_text_input_v1_commit(get());
}

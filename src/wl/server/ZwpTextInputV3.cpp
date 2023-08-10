// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpTextInputV3.h"

using namespace wl::server;

const struct zwp_text_input_v3_interface ZwpTextInputV3::impl = {
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_destroy>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_enable>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_disable>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_set_surrounding_text>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_set_text_change_cause>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_set_content_type>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_set_cursor_rectangle>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::zwp_text_input_v3_commit>::func,
};

ZwpTextInputV3::ZwpTextInputV3()
    : Type()
{
}

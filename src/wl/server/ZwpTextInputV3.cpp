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

void ZwpTextInputV3::send_enter(struct wl_resource *resource, struct wl_resource *surface)
{
    zwp_text_input_v3_send_enter(resource, surface);
}

void ZwpTextInputV3::send_leave(struct wl_resource *resource, struct wl_resource *surface)
{
    zwp_text_input_v3_send_leave(resource, surface);
}

void ZwpTextInputV3::send_preedit_string(struct wl_resource *resource,
                                         const char *text,
                                         int32_t cursor_begin,
                                         int32_t cursor_end)
{
    zwp_text_input_v3_send_preedit_string(resource, text, cursor_begin, cursor_end);
}

void ZwpTextInputV3::send_commit_string(struct wl_resource *resource, const char *text)
{
    zwp_text_input_v3_send_commit_string(resource, text);
}

void ZwpTextInputV3::send_delete_surrounding_text(struct wl_resource *resource,
                                                  uint32_t before_length,
                                                  uint32_t after_length)
{
    zwp_text_input_v3_send_delete_surrounding_text(resource, before_length, after_length);
}

void ZwpTextInputV3::send_done(struct wl_resource *resource, uint32_t serial)
{
    zwp_text_input_v3_send_done(resource, serial);
}

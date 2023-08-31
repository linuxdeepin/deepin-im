// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpDimTextInputV1.h"

using namespace wl::server;

const struct zwp_dim_text_input_v1_interface ZwpDimTextInputV1::impl = {
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_destroy>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_enable>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_disable>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_set_surrounding_text>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_set_text_change_cause>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_set_content_type>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_set_cursor_rectangle>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputV1::zwp_dim_text_input_v1_commit>::func,
};

ZwpDimTextInputV1::ZwpDimTextInputV1()
    : Type()
{
}

void ZwpDimTextInputV1::send_enter(struct wl_resource *resource)
{
    zwp_dim_text_input_v1_send_enter(resource);
}

void ZwpDimTextInputV1::send_leave(struct wl_resource *resource)
{
    zwp_dim_text_input_v1_send_leave(resource);
}

void ZwpDimTextInputV1::send_modifiers_map(struct wl_resource *resource, struct wl_array *map)
{
    zwp_dim_text_input_v1_send_modifiers_map(resource, map);
}

void ZwpDimTextInputV1::send_preedit_string(struct wl_resource *resource,
                                            const char *text,
                                            int32_t cursor_begin,
                                            int32_t cursor_end)
{
    zwp_dim_text_input_v1_send_preedit_string(resource, text, cursor_begin, cursor_end);
}

void ZwpDimTextInputV1::send_commit_string(struct wl_resource *resource, const char *text)
{
    zwp_dim_text_input_v1_send_commit_string(resource, text);
}

void ZwpDimTextInputV1::send_delete_surrounding_text(struct wl_resource *resource,
                                                     uint32_t before_length,
                                                     uint32_t after_length)
{
    zwp_dim_text_input_v1_send_delete_surrounding_text(resource, before_length, after_length);
}

void ZwpDimTextInputV1::send_done(struct wl_resource *resource, uint32_t serial)
{
    zwp_dim_text_input_v1_send_done(resource, serial);
}

void ZwpDimTextInputV1::send_keysym(struct wl_resource *resource,
                                    uint32_t serial,
                                    uint32_t time,
                                    uint32_t sym,
                                    uint32_t state,
                                    uint32_t modifiers)
{
    zwp_dim_text_input_v1_send_keysym(resource, serial, time, sym, state, modifiers);
}

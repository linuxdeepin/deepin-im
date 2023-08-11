// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodV2.h"

using namespace wl::server;

template<typename T, typename U>
T base_of(U T::*);

const struct zwp_input_method_v2_interface ZwpInputMethodV2::impl = {
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_commit_string>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_set_preedit_string>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_delete_surrounding_text>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_commit>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_get_input_popup_surface>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_grab_keyboard>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::zwp_input_method_v2_destroy>::func,
};

ZwpInputMethodV2::ZwpInputMethodV2()
    : Type()
{
}

void ZwpInputMethodV2::send_activate(struct wl_resource *resource)
{
    zwp_input_method_v2_send_activate(resource);
}

void ZwpInputMethodV2::send_deactivate(struct wl_resource *resource)
{
    zwp_input_method_v2_send_deactivate(resource);
}

void ZwpInputMethodV2::send_surrounding_text(struct wl_resource *resource, const char *text, uint32_t cursor, uint32_t anchor) {}

void ZwpInputMethodV2::send_text_change_cause(struct wl_resource *resource, uint32_t cause) {}

void ZwpInputMethodV2::send_content_type(struct wl_resource *resource, uint32_t hint, uint32_t purpose) {}

void ZwpInputMethodV2::send_done(struct wl_resource *resource) {}

void ZwpInputMethodV2::send_unavailable(struct wl_resource *resource) {}


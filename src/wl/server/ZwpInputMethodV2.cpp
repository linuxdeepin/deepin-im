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

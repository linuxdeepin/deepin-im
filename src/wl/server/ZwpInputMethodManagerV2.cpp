// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodManagerV2.h"

using namespace wl::server;

const struct zwp_input_method_manager_v2_interface ZwpInputMethodManagerV2::impl = {
    ResourceCallbackWrapper<&ZwpInputMethodManagerV2::zwp_input_method_manager_v2_get_input_method>::func,
    ResourceCallbackWrapper<&ZwpInputMethodManagerV2::zwp_input_method_manager_v2_destroy>::func,
};

ZwpInputMethodManagerV2::ZwpInputMethodManagerV2()
    : Type()
{
}

ZwpInputMethodManagerV2::~ZwpInputMethodManagerV2() { }

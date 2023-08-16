// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpDimTextInputManagerV1.h"

using namespace wl::server;

const struct zwp_dim_text_input_manager_v1_interface ZwpDimTextInputManagerV1::impl = {
    ResourceCallbackWrapper<&ZwpDimTextInputManagerV1::zwp_dim_text_input_manager_v1_destroy>::func,
    ResourceCallbackWrapper<&ZwpDimTextInputManagerV1::zwp_dim_text_input_manager_v1_get_text_input>::func,
};

ZwpDimTextInputManagerV1::ZwpDimTextInputManagerV1()
    : Type()
{
}

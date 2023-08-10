// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpTextInputManagerV3.h"

using namespace wl::server;

const struct zwp_text_input_manager_v3_interface ZwpTextInputManagerV3::impl = {
    ResourceCallbackWrapper<&ZwpTextInputManagerV3::zwp_text_input_manager_v3_destroy>::func,
    ResourceCallbackWrapper<&ZwpTextInputManagerV3::zwp_text_input_manager_v3_get_text_input>::func,
};

ZwpTextInputManagerV3::ZwpTextInputManagerV3()
    : Type()
{
}

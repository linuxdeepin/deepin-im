// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpTextInputManagerV3.h"

using namespace wl::server;

template<>
const struct zwp_text_input_manager_v3_interface ZwpTextInputManagerV3::Type::impl = {
    ResourceCallbackWrapper<&ZwpTextInputManagerV3::destroy>::func,
    ResourceCallbackWrapper<&ZwpTextInputManagerV3::getTextInput>::func,
};

ZwpTextInputManagerV3::ZwpTextInputManagerV3(struct wl_client *client, uint32_t id)
    : Type(client, id)
{
}

void ZwpTextInputManagerV3::destroy([[maybe_unused]] struct wl_client *client) { }

void ZwpTextInputManagerV3::getTextInput([[maybe_unused]] struct wl_client *client,
                                         [[maybe_unused]] uint32_t id,
                                         [[maybe_unused]] struct wl_resource *seat)
{
}

void ZwpTextInputManagerV3::resourceDestroy() { }
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodV2.h"

using namespace wl::server;

template<typename T, typename U>
T base_of(U T::*);

template<>
const struct zwp_input_method_v2_interface ZwpInputMethodV2::Type::impl = {
    ResourceCallbackWrapper<&ZwpInputMethodV2::commitString>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::setPreeditString>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::deleteSurroundingText>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::commit>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::getInputPopupSurface>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::grabKeyboard>::func,
    ResourceCallbackWrapper<&ZwpInputMethodV2::destroy>::func,
};

ZwpInputMethodV2::ZwpInputMethodV2(wl_client *client, uint32_t id)
    : Type(client, id)
{
}

void ZwpInputMethodV2::commitString([[maybe_unused]] struct wl_client *client,
                                    [[maybe_unused]] const char *text)
{
}

void ZwpInputMethodV2::setPreeditString([[maybe_unused]] struct wl_client *client,
                                        [[maybe_unused]] const char *text,
                                        [[maybe_unused]] int32_t cursor_begin,
                                        [[maybe_unused]] int32_t cursor_end)
{
}

void ZwpInputMethodV2::deleteSurroundingText([[maybe_unused]] struct wl_client *client,
                                             [[maybe_unused]] uint32_t before_length,
                                             [[maybe_unused]] uint32_t after_length)
{
}

void ZwpInputMethodV2::commit([[maybe_unused]] struct wl_client *client,
                              [[maybe_unused]] uint32_t serial)
{
}

void ZwpInputMethodV2::getInputPopupSurface([[maybe_unused]] struct wl_client *client,
                                            [[maybe_unused]] uint32_t id,
                                            [[maybe_unused]] struct wl_resource *surface)
{
}

void ZwpInputMethodV2::grabKeyboard([[maybe_unused]] struct wl_client *client,
                                    [[maybe_unused]] uint32_t keyboard)
{
}

void ZwpInputMethodV2::destroy([[maybe_unused]] struct wl_client *client) { }

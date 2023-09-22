// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputPopupSurfaceV2.h"

using namespace wl::server;

const struct zwp_input_popup_surface_v2_interface ZwpInputPopupSurfaceV2::impl = {
    ResourceCallbackWrapper<&ZwpInputPopupSurfaceV2::zwp_input_popup_surface_v2_destroy>::func,
};

ZwpInputPopupSurfaceV2::ZwpInputPopupSurfaceV2()
    : Type()
{
}

ZwpInputPopupSurfaceV2::~ZwpInputPopupSurfaceV2() = default;

void ZwpInputPopupSurfaceV2::send_text_input_rectangle(
    struct wl_resource *resource, int32_t x, int32_t y, int32_t width, int32_t height)
{
    zwp_input_popup_surface_v2_send_text_input_rectangle(resource, x, y, width, height);
}

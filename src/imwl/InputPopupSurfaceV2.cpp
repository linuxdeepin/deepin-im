// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputPopupSurfaceV2.h"

InputPopupSurfaceV2::InputPopupSurfaceV2()
    : wl::server::ZwpInputPopupSurfaceV2()
{
}

InputPopupSurfaceV2::~InputPopupSurfaceV2() = default;

void InputPopupSurfaceV2::sendTextInputRectangle(int32_t x,
                                                 int32_t y,
                                                 int32_t width,
                                                 int32_t height)
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        send_text_input_rectangle(resource->handle, x, y, width, height);
    }
}

void InputPopupSurfaceV2::zwp_input_popup_surface_v2_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

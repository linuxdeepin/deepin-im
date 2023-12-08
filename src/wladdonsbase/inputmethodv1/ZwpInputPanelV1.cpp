// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputPanelV1.h"

WL_ADDONS_BASE_USE_NAMESPACE

const struct zwp_input_panel_v1_interface ZwpInputPanelV1::impl = {
    ResourceCallbackWrapper<&ZwpInputPanelV1::zwp_input_panel_v1_get_input_panel_surface>::func,
};

ZwpInputPanelV1::ZwpInputPanelV1()
    : Type()
{
}

ZwpInputPanelV1::~ZwpInputPanelV1() = default;

void ZwpInputPanelV1::zwp_input_panel_v1_get_input_panel_surface(Resource *resource,
                                                                 uint32_t id,
                                                                 struct wl_resource *surface)
{
}

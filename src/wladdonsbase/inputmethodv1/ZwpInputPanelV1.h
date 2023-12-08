// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_ADDONS_BASE_ZWPINPUTPANELV1_H
#define WL_ADDONS_BASE_ZWPINPUTPANELV1_H

#include "Type.h"
#include "wayland-input-method-unstable-v1-server-protocol.h"

WL_ADDONS_BASE_BEGIN_NAMESPACE

class ZwpInputPanelV1 : public Type<ZwpInputPanelV1, zwp_input_panel_v1>
{
    friend class Type;

public:
    ZwpInputPanelV1();
    ~ZwpInputPanelV1() override;

protected:
    void zwp_input_panel_v1_get_input_panel_surface(Resource *resource,
                                                    uint32_t id,
                                                    struct wl_resource *surface);

private:
    static const struct zwp_input_panel_v1_interface impl;
};

WL_ADDONS_BASE_END_NAMESPACE

#endif // !WL_ADDONS_BASE_ZWPINPUTPANELV1_H

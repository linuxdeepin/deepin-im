// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPINPUTPOPUPSURFACEV2_H
#define WL_SERVER_ZWPINPUTPOPUPSURFACEV2_H

#include "Type.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"

#include <memory>

namespace wl {
namespace server {

class ZwpInputPopupSurfaceV2 : public Type<ZwpInputPopupSurfaceV2, zwp_input_popup_surface_v2>
{
    friend class Type;

public:
    ZwpInputPopupSurfaceV2();
    ~ZwpInputPopupSurfaceV2() override;

    void send_text_input_rectangle(struct wl_resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);

protected:
    virtual void zwp_input_popup_surface_v2_destroy(Resource *resource) = 0;

private:
    static const struct zwp_input_popup_surface_v2_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPINPUTPOPUPSURFACEV2_H

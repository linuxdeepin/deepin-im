// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPDIMTEXTINPUTMANAGERV1_H
#define WL_SERVER_ZWPDIMTEXTINPUTMANAGERV1_H

#include "Type.h"
#include "wayland-dim-text-input-unstable-v1-server-protocol.h"

namespace wl {
namespace server {

class ZwpDimTextInputManagerV1 : public Type<ZwpDimTextInputManagerV1, zwp_dim_text_input_manager_v1>
{
    friend class Type;

public:
    ZwpDimTextInputManagerV1();

protected:
    virtual void zwp_dim_text_input_manager_v1_destroy(Resource *resource) = 0;
    virtual void zwp_dim_text_input_manager_v1_get_text_input(Resource *resource, uint32_t id, struct ::wl_resource *seat) = 0;

private:
    static const struct zwp_dim_text_input_manager_v1_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPDIMTEXTINPUTMANAGERV1_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPTEXTINPUTMANAGERV3_H
#define WL_SERVER_ZWPTEXTINPUTMANAGERV3_H

#include "Type.h"
#include "wayland-text-input-unstable-v3-server-protocol.h"

namespace wl {
namespace server {

class ZwpTextInputManagerV3 : public Type<ZwpTextInputManagerV3, zwp_text_input_manager_v3>
{
    friend class Type;

public:
    ZwpTextInputManagerV3();

protected:
    virtual void zwp_text_input_manager_v3_destroy(Resource *resource) = 0;
    virtual void zwp_text_input_manager_v3_get_text_input(Resource *resource, uint32_t id, struct ::wl_resource *seat) = 0;

private:
    static const struct zwp_text_input_manager_v3_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPTEXTINPUTMANAGERV3_H

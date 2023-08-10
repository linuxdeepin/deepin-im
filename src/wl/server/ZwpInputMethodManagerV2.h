// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPINPUTMETHODMANAGERV2_H
#define WL_SERVER_ZWPINPUTMETHODMANAGERV2_H

#include "Type.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"

#include <memory>

namespace wl {
namespace server {

class Resource;
class Seat;
class ZwpInputMethodV2;

class ZwpInputMethodManagerV2 : public Type<ZwpInputMethodManagerV2, zwp_input_method_manager_v2>
{
    friend class Type;

public:
    ZwpInputMethodManagerV2();
    virtual ~ZwpInputMethodManagerV2();

protected:
    virtual void zwp_input_method_manager_v2_get_input_method(Resource *resource, struct ::wl_resource *seat, uint32_t input_method) = 0;
    virtual void zwp_input_method_manager_v2_destroy(Resource *resource) = 0;

private:
    static const struct zwp_input_method_manager_v2_interface impl;
};

} // namespace server
}; // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODMANAGERV2_H

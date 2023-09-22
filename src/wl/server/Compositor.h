// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_COMPOSITOR_H
#define WL_SERVER_COMPOSITOR_H

#include "Type.h"

#include <wayland-server-protocol.h>

namespace wl::server {

class Compositor : public Type<Compositor, wl_compositor>
{
    friend class Type;

public:
    Compositor();
    ~Compositor() override;

protected:
    virtual void wl_compositor_create_surface(Resource *resource, uint32_t id) = 0;
    virtual void wl_compositor_create_region(Resource *resource, uint32_t id) = 0;

private:
    static const struct wl_compositor_interface impl;
};

} // namespace wl::server

#endif // !WL_SERVER_COMPOSITOR_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_SURFACE_H
#define WL_SERVER_SURFACE_H

#include "Type.h"

#include <wayland-server-protocol.h>

namespace wl::server {

class Surface : public Type<Surface, wl_surface>
{
    friend class Type;

public:
    Surface();
    virtual ~Surface();

protected:
    virtual void wl_surface_destroy(Resource *resource) = 0;
    virtual void wl_surface_attach(Resource *resource, struct wl_resource *buffer, int32_t x, int32_t y) = 0;
    virtual void wl_surface_damage(Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) = 0;
    virtual void wl_surface_frame(Resource *resource, uint32_t callback) = 0;
    virtual void wl_surface_set_opaque_region(Resource *resource, struct wl_resource *region) = 0;
    virtual void wl_surface_set_input_region(Resource *resource, struct wl_resource *region) = 0;
    virtual void wl_surface_commit(Resource *resource) = 0;
    virtual void wl_surface_set_buffer_transform(Resource *resource, int32_t transform) = 0;
    virtual void wl_surface_set_buffer_scale(Resource *resource, int32_t scale) = 0;
    virtual void wl_surface_damage_buffer(Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) = 0;
    virtual void wl_surface_offset(Resource *resource, int32_t x, int32_t y) = 0;

private:
    static const struct wl_surface_interface impl;
};

} // namespace wl::server

#endif // !WL_SERVER_SURFACE_H

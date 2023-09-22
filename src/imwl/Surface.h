// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SURFACE_H
#define SURFACE_H

#include "wl/server/Surface.h"

#include <memory>

namespace wl::server {
class Resource;
}

class Surface : public wl::server::Surface
{
public:
    Surface();
    ~Surface() override;

protected:
    void wl_surface_destroy(wl::server::Resource *resource) override;
    void wl_surface_attach(wl::server::Resource *resource, struct wl_resource *buffer, int32_t x, int32_t y) override;
    void wl_surface_damage(wl::server::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) override;
    void wl_surface_frame(wl::server::Resource *resource, uint32_t callback) override;
    void wl_surface_set_opaque_region(wl::server::Resource *resource, struct wl_resource *region) override;
    void wl_surface_set_input_region(wl::server::Resource *resource, struct wl_resource *region) override;
    void wl_surface_commit(wl::server::Resource *resource) override;
    void wl_surface_set_buffer_transform(wl::server::Resource *resource, int32_t transform) override;
    void wl_surface_set_buffer_scale(wl::server::Resource *resource, int32_t scale) override;
    void wl_surface_damage_buffer(wl::server::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) override;
    void wl_surface_offset(wl::server::Resource *resource, int32_t x, int32_t y) override;

private:
};

#endif // SURFACE_H

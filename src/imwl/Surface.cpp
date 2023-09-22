// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Surface.h"

#include <QDebug>

Surface::Surface()
    : wl::server::Surface()
{
}

Surface::~Surface() = default;

void Surface::wl_surface_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

void Surface::wl_surface_attach(wl::server::Resource *resource,
                                struct wl_resource *buffer,
                                int32_t x,
                                int32_t y)
{
    qDebug() << "wl_surface_attach";
}

void Surface::wl_surface_damage(
    wl::server::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height)
{
    qDebug() << "wl_surface_damage";
}

void Surface::wl_surface_frame(wl::server::Resource *resource, uint32_t callback)
{
    qDebug() << "wl_surface_frame";
}

void Surface::wl_surface_set_opaque_region(wl::server::Resource *resource,
                                           struct wl_resource *region)
{
    qDebug() << "wl_surface_set_opaque_region";
}

void Surface::wl_surface_set_input_region(wl::server::Resource *resource,
                                          struct wl_resource *region)
{
    qDebug() << "wl_surface_set_input_region";
}

void Surface::wl_surface_commit(wl::server::Resource *resource)
{
    qDebug() << "wl_surface_commit";
}

void Surface::wl_surface_set_buffer_transform(wl::server::Resource *resource, int32_t transform)
{
    qDebug() << "wl_surface_set_buffer_transform";
}

void Surface::wl_surface_set_buffer_scale(wl::server::Resource *resource, int32_t scale)
{
    qDebug() << "wl_surface_set_buffer_scale";
}

void Surface::wl_surface_damage_buffer(
    wl::server::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height)
{
    qDebug() << "wl_surface_damage_buffer";
}

void Surface::wl_surface_offset(wl::server::Resource *resource, int32_t x, int32_t y)
{
    qDebug() << "wl_surface_offset";
}

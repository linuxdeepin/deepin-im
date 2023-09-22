// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Compositor.h"

#include "Surface.h"

#include <QDebug>

Compositor::Compositor()
    : wl::server::Compositor()
    , surface_(std::make_unique<Surface>())
{
}

Compositor::~Compositor() = default;

void Compositor::wl_compositor_create_surface(wl::server::Resource *resource, uint32_t id)
{
    surface_->add(resource->client(), id);
}

void Compositor::wl_compositor_create_region(wl::server::Resource *resource, uint32_t id)
{
    qDebug() << "wl_compositor_create_region";
}

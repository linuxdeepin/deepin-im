// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "wl/server/Compositor.h"

#include <memory>

namespace wl::server {
class Resource;
}

class Surface;

class Compositor : public wl::server::Compositor
{
public:
    Compositor();
    ~Compositor() override;

protected:
    void wl_compositor_create_surface(wl::server::Resource *resource, uint32_t id) override;
    void wl_compositor_create_region(wl::server::Resource *resource, uint32_t id) override;

private:
    std::unique_ptr<Surface> surface_;
};

#endif // COMPOSITOR_H

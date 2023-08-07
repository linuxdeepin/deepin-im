// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_RESOURCE_H
#define WL_RESOURCE_H

#include "common/common.h"

#include <wayland-server-core.h>

#include <memory>

namespace wl {
namespace server {

class Resource
{
public:
    Resource(struct wl_client *client,
             const struct wl_interface *interface,
             int version,
             uint32_t id);

    void setImplementation(const void *implementation,
                           void *data,
                           wl_resource_destroy_func_t destroy);

private:
    std::unique_ptr<wl_resource, Deleter<wl_resource_destroy>> resource_;
};

} // namespace server
} // namespace wl

#endif // !WL_RESOURCE_H

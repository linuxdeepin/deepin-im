// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_TYPE_H
#define WL_SERVER_TYPE_H

#include "Resource.h"
#include "common/common.h"
#include "wl/Type.h"

#include <wayland-server-core.h>

#include <map>
#include <memory>

#include <assert.h>

namespace wl {
namespace server {

template<auto F>
struct GlobalCallbackWrapper;

template<typename C, typename R, typename Arg, typename... Args, R (C::*F)(Arg, Args...)>
struct GlobalCallbackWrapper<F>
{
    static R func(Arg farg, void *userdata, Args... args)
    {
        auto *p = reinterpret_cast<C *>(userdata);
        return (p->*F)(farg, args...);
    }
};

template<typename C, typename T>
class Type : public wl::Type<T>
{
public:
    Type() = default;

    virtual ~Type() = default;

    void init(struct wl_display *display)
    {
        global_.reset(wl_global_create(display,
                                       this->wl_interface,
                                       this->wl_interface->version,
                                       this,
                                       GlobalCallbackWrapper<&Type::bindCb>::func));
    }

    void add(struct wl_client *client, uint32_t id)
    {
        auto resource =
            std::make_shared<Resource>(client, this->wl_interface, this->wl_interface->version, id);
        resource->setImplementation(&C::impl,
                                    this,
                                    ResourceDestroyWrapper<&Type::resourceDestroyCb>::func);

        resourceMap_.emplace(client, resource);
    }

    static C *fromResource(struct wl_resource *resource)
    {
        auto *r = static_cast<Resource *>(wl_resource_get_user_data(resource));
        return static_cast<C *>(r->object());
    }

protected:
    const auto resourceMap() { return resourceMap_; }

    virtual void resource_destroy(Resource *resource) { }

private:
    std::unique_ptr<struct wl_global, Deleter<wl_global_destroy>> global_;
    std::multimap<struct wl_client *, std::shared_ptr<Resource>> resourceMap_;

    void bindCb(struct wl_client *client, uint32_t version, uint32_t id) { }

    void resourceDestroyCb(Resource *resource)
    {
        resource_destroy(resource);

        auto iterpair = resourceMap_.equal_range(resource->client());
        auto it = iterpair.first;
        for (; it != iterpair.second; ++it) {
            if (it->second.get() == resource) {
                resourceMap_.erase(it);
                break;
            }
        }
    }
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_TYPE_H

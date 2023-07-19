#ifndef WL_RESOURCE_H
#define WL_RESOURCE_H

#include "common/common.h"
#include "utils.h"

#include <wayland-server-core.h>

#include <memory>

namespace wl {
namespace server {

class Resource
{
public:
    template<typename T>
    static std::shared_ptr<Resource> create(wl_client *client, uint32_t id)
    {
        auto r = std::make_shared<Resource>(client, T::interface, T::interface.version, id);

        return r;
    }

    void setImplementation(const void *implementation,
                           void *data,
                           wl_resource_destroy_func_t destroy);

private:
    std::unique_ptr<wl_resource, Deleter<wl_resource_destroy>> resource_;

    Resource(struct wl_client *client,
             const struct wl_interface *interface,
             int version,
             uint32_t id);
};

} // namespace server
} // namespace wl

#endif // !WL_RESOURCE_H

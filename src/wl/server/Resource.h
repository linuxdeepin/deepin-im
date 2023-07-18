#ifndef WL_RESOURCE_H
#define WL_RESOURCE_H

#include "common/common.h"
#include "utils.h"

#include <wayland-server-core.h>

#include <memory>

namespace wl {

class Resource
{
public:
    template<typename T>
    std::shared_ptr<Resource> create(wl_client *client, uint32_t id)
    {
        auto r = std::make_shared<Resource>(client, T::interface, T::interface.version, id);
        r->val_ = std::make_shared<T>();
        wl_resource_set_implementation(r->v_,
                                       T::impl,
                                       r->val_.get(),
                                       ResourceDestrouWrapper<&Resource::onDestroy>::func);

        return r;
    }

private:
    std::unique_ptr<wl_resource, Deleter<wl_resource_destroy>> resource_;
    std::shared_ptr<void> val_;

    Resource(struct wl_client *client,
             const struct wl_interface *interface,
             int version,
             uint32_t id);

    void onDestroy();
};

} // namespace wl

#endif // !WL_RESOURCE_H

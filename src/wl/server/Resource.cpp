#include "Resource.h"

using namespace wl::server;

Resource::Resource(struct wl_client *client,
                   const struct wl_interface *interface,
                   int version,
                   uint32_t id)
    : resource_(wl_resource_create(client, interface, version, id))
{
}

void Resource::setImplementation(const void *implementation,
                                 void *data,
                                 wl_resource_destroy_func_t destroy)
{
    wl_resource_set_implementation(resource_.get(), implementation, data, destroy);
}

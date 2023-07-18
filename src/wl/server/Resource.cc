#include "Resource.h"

using namespace wl;

Resource::Resource(struct wl_client *client,
                   const struct wl_interface *interface,
                   int version,
                   uint32_t id)
    : resource_(wl_resource_create(client, interface, version, id))
{
    wl_resource_set_user_data(resource_.get(), this);
}

void Resource::onDestroy() {

}

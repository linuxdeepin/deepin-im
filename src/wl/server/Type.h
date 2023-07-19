#ifndef WL_SERVER_TYPE_H
#define WL_SERVER_TYPE_H

#include "Resource.h"
#include "utils.h"
#include "wl/Type.h"

#include <wayland-server-core.h>

#include <memory>

namespace wl {
namespace server {

class Resource;

template<typename T, typename I>
class Type : public wl::Type<T>
{
public:
    Type(wl_client *client, uint32_t id)
        : resource_(
            std::make_shared<Resource>(client, this->wl_interface, this->wl_interface->version, id))
    {
        resource_->setImplementation(&impl,
                                     this,
                                     ResourceDestroyWrapper<&Type::resourceDestroy>::func);
    }

protected:
    static const I impl;

    const std::shared_ptr<Resource> &resource() { return resource_; }

private:
    std::shared_ptr<Resource> resource_;

    virtual void resourceDestroy() = 0;
};

} // namespace server
} // namespace wl

#define BASE_TYPE(T) wl::server::Type<T, struct T##_interface>

#endif // !WL_SERVER_TYPE_H

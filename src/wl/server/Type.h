#ifndef WL_SERVER_TYPE_H
#define WL_SERVER_TYPE_H

#include "wl/Type.h"

namespace wl {
namespace server {

template<typename T, typename I>
class Type : public wl::Type<T>
{
protected:
    static const I impl;
};

} // namespace server
} // namespace wl

#define BASE_TYPE(T) wl::server::Type<T, struct T##_interface>

#endif // !WL_SERVER_TYPE_H

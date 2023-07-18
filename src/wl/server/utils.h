#ifndef WL_SERVER_UTILS_H
#define WL_SERVER_UTILS_H

#include <wayland-server-core.h>

template<auto F>
struct ResourceDestrouWrapper;

template<typename C, void (C::*F)()>
struct ResourceDestrouWrapper<F>
{
    static void func(struct wl_resource *resource)
    {
        auto *p = static_cast<C *>(wl_resource_get_user_data(resource));
        (p->*F)();
    }
};

#endif // !WL_SERVER_UTILS_H

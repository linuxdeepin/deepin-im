#ifndef WL_SERVER_UTILS_H
#define WL_SERVER_UTILS_H

#include <wayland-server-core.h>

template<auto F>
struct ResourceCallbackWrapper;

template<typename C, typename R, typename... Args, R (C::*F)(wl_client *client, Args...)>
struct ResourceCallbackWrapper<F>
{
    static R func(wl_client *client, wl_resource *resource, Args... args)
    {
        auto *p = static_cast<C *>(wl_resource_get_user_data(resource));
        return (p->*F)(client, args...);
    }
};

template<auto F>
struct ResourceDestroyWrapper;

template<typename C, void (C::*F)()>
struct ResourceDestroyWrapper<F>
{
    static void func(struct wl_resource *resource)
    {
        auto *p = static_cast<C *>(wl_resource_get_user_data(resource));
        (p->*F)();
    }
};

#endif // !WL_SERVER_UTILS_H

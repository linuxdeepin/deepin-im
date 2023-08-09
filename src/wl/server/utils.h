// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
struct EventCallbackWrapper;

template<typename C, typename... Args, void (C::*F)(Args...)>
struct EventCallbackWrapper<F>
{
    static void func(Args... args, void *data)
    {
        auto *p = static_cast<C *>(data);
        (p->*F)(args...);
    }
};

#endif // !WL_SERVER_UTILS_H

#ifndef WL_CLIENT_UTILS_H
#define WL_CLIENT_UTILS_H

template<auto F>
struct CallbackWrapper;

template<typename C, typename R, typename... Args, R (C::*F)(Args...)>
struct CallbackWrapper<F>
{
    static R func(void *userdata, Args... args)
    {
        auto *p = static_cast<C *>(userdata);
        return (p->*F)(args...);
    }
};

#endif // !WL_CLIENT_UTILS_H

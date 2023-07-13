#ifndef WLFRONTEND_UTILS_H
#define WLFRONTEND_UTILS_H

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

#endif // !WLFRONTEND_UTILS_H

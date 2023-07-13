#ifndef QTFRONTEND_UTILS_H
#define QTFRONTEND_UTILS_H

template<auto F>
struct CallbackWrapper;

template<typename C, typename R, typename... Args, R (C::*F)(Args...)>
struct CallbackWrapper<F>
{
    static R func(void *userdata, Args... args)
    {
        auto *p = reinterpret_cast<C *>(userdata);
        return (p->*F)(args...);
    }
};

#endif // !QTFRONTEND_UTILS_H

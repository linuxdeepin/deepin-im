#ifndef UTILS_H
#define UTILS_H

template<auto F>
struct CallbackWrapper;

template<typename C, typename R, typename Arg, typename... Args, R (C::*F)(Arg, Args...)>
struct CallbackWrapper<F>
{
    static R func(Arg farg, void *userdata, Args... args)
    {
        auto *p = reinterpret_cast<C *>(userdata);
        return (p->*F)(farg, args...);
    }
};

#endif // !UTILS_H

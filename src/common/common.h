#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

template<typename T>
inline constexpr bool always_false_v = false;

template<auto Func>
class Deleter
{
public:
    template<typename T>
    void operator()(T *ptr) const
    {
        if (ptr) {
            Func(ptr);
        }
    }
};

#endif // COMMON_COMMON_H_
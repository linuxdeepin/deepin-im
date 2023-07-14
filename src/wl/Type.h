#ifndef WLTYPE_H
#define WLTYPE_H

#include "common/common.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"
#include "wayland-virtual-keyboard-unstable-v1-client-protocol.h"

#include <wayland-client-protocol.h>

#include <type_traits>

#include <stdint.h>

namespace wl {

template<typename T>
class Type
{
public:
    static constexpr uint32_t key()
    {
        if constexpr (std::is_same_v<T, wl_seat>) {
            return 1;
        } else if constexpr (std::is_same_v<T, zwp_input_method_manager_v2>) {
            return 2;
        } else if constexpr (std::is_same_v<T, zwp_input_method_v2>) {
            return 3;
        } else if constexpr (std::is_same_v<T, zwp_virtual_keyboard_manager_v1>) {
            return 4;
        } else {
            static_assert(always_false_v<T>, "unknown type");
        }
    };

    Type(T *val)
        : val_(val)
    {
    }

    ~Type()
    {
        if constexpr (std::is_same_v<T, wl_seat>) {
            uint32_t version = wl_seat_get_version(val_);
            if (version >= 5) {
                wl_seat_release(val_);
            } else {
                wl_seat_destroy(val_);
            }
        } else if constexpr (std::is_same_v<T, zwp_input_method_manager_v2>) {
            zwp_input_method_manager_v2_destroy(val_);
        } else if constexpr (std::is_same_v<T, zwp_input_method_v2>) {
            zwp_input_method_v2_destroy(val_);
        } else if constexpr (std::is_same_v<T, zwp_virtual_keyboard_manager_v1>) {
            zwp_virtual_keyboard_manager_v1_destroy(val_);
        } else if constexpr (std::is_same_v<T, zwp_virtual_keyboard_v1>) {
            auto version = zwp_virtual_keyboard_v1_get_version(val_);
            if (version >= 1) {
                zwp_virtual_keyboard_v1_destroy(val_);
            }
        } else if constexpr (std::is_same_v<T, zwp_input_method_keyboard_grab_v2>) {
            zwp_input_method_keyboard_grab_v2_destroy(val_);
        } else {
            static_assert(always_false_v<T>, "unknown type");
        }
    }

    T *get() { return val_; }

private:
    T *val_;
};

} // namespace wl

#endif // !WLTYPE_H

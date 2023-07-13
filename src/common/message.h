
#ifndef _ORG_DEEPIN_DIM_DBUS_MESSAGE_H_
#define _ORG_DEEPIN_DIM_DBUS_MESSAGE_H_

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace org {
namespace deepin {
namespace dim {
namespace dbus {

/**
 * A type to represent DBus struct.
 *
 * It is used for message serialization.
 */
template<typename... Args>
struct DBusStruct
{
    typedef std::tuple<Args...> tuple_type;

    DBusStruct() = default;

    template<typename Element,
             typename... Elements,
             typename = typename std::enable_if_t<
                 sizeof...(Elements) != 0
                 || !std::is_same<typename std::decay_t<Element>, DBusStruct>::value>>
    DBusStruct(Element &&ele, Elements &&...elements)
        : data_(std::forward<Element>(ele), std::forward<Elements>(elements)...)
    {
    }

    DBusStruct(const DBusStruct &) = default;
    DBusStruct(DBusStruct &&) noexcept(std::is_nothrow_move_constructible<tuple_type>::value) =
        default;
    DBusStruct &operator=(const DBusStruct &other) = default;
    DBusStruct &operator=(DBusStruct &&other) noexcept(
        std::is_nothrow_move_assignable<tuple_type>::value) = default;

    explicit DBusStruct(const tuple_type &other)
        : data_(std::forward(other))
    {
    }

    explicit DBusStruct(tuple_type &&other)
        : data_(std::forward<tuple_type>(other))
    {
    }

    constexpr tuple_type &data() { return data_; }

    constexpr const tuple_type &data() const { return data_; }

private:
    tuple_type data_;
};

} // namespace dbus
} // namespace dim
} // namespace deepin
} // namespace org

namespace std {

template<std::size_t i, typename... _Elements>
constexpr auto &get(org::deepin::dim::dbus::DBusStruct<_Elements...> &t) noexcept
{
    return std::get<i>(t.data());
}

template<std::size_t i, typename... _Elements>
constexpr auto &get(const org::deepin::dim::dbus::DBusStruct<_Elements...> &t) noexcept
{
    return std::get<i>(t.data());
}

template<typename T, typename... _Elements>
constexpr auto &get(org::deepin::dim::dbus::DBusStruct<_Elements...> &t) noexcept
{
    return std::get<T>(t.data());
}

template<typename T, typename... _Elements>
constexpr auto &get(const org::deepin::dim::dbus::DBusStruct<_Elements...> &t) noexcept
{
    return std::get<T>(t.data());
}
} // namespace std

#endif // _ORG_DEEPIN_DIM_DBUS_MESSAGE_H_
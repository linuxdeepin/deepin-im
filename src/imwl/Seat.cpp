#include "Seat.h"

#include "InputMethodV2.h"
#include "TextInputV3.h"
#include "VirtualKeyboardV1.h"

#include <QDebug>

Seat::Seat()
    : wl::server::Seat()
    , textInputV3_(std::make_shared<TextInputV3>(this))
    , inputMethodV2_(std::make_shared<InputMethodV2>(this))
    , virtualKeyboardV1_(std::make_shared<VirtualKeyboardV1>(this))
{
}

Seat::~Seat() { }

void Seat::wl_seat_get_pointer(wl::server::Resource *resource, uint32_t id)
{
    qWarning() << "wl_seat_get_pointer";
}

void Seat::wl_seat_get_keyboard(wl::server::Resource *resource, uint32_t id)
{
    qWarning() << "wl_seat_get_keybpard";
}

void Seat::wl_seat_get_touch(wl::server::Resource *resource, uint32_t id)
{
    qWarning() << "wl_seat_get_touch";
}

void Seat::wl_seat_release(wl::server::Resource *resource)
{
    qWarning() << "wl_seat_release";
}

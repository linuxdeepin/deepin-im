#include "Seat.h"

#include <QDebug>

Seat::Seat()
    : wl::server::Seat()
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

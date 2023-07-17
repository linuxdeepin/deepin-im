#include "Seat.h"

using namespace wl;

Seat::Seat(wl_seat *val)
    : Type(val)
{
}

Seat::~Seat()
{
    uint32_t version = wl_seat_get_version(get());
    if (version >= 5) {
        wl_seat_release(get());
    } else {
        wl_seat_destroy(get());
    }
}

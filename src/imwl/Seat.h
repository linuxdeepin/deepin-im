#ifndef IMWL_SEAT_H
#define IMWL_SEAT_H

#include "wl/server/Seat.h"

class Seat : public wl::server::Seat
{
public:
    Seat();
    virtual ~Seat();

protected:
    void wl_seat_get_pointer(wl::server::Resource *resource, uint32_t id) override;
    void wl_seat_get_keyboard(wl::server::Resource *resource, uint32_t id) override;
    void wl_seat_get_touch(wl::server::Resource *resource, uint32_t id) override;
    void wl_seat_release(wl::server::Resource *resource) override;
};

#endif // !IMWL_SEAT_H

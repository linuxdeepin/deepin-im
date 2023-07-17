#ifndef WL_SEAT_H
#define WL_SEAT_H

#include "Type.h"

namespace wl {

class Seat : public Type<wl_seat>
{
public:
    Seat(wl_seat *val);
    ~Seat();
};

} // namespace wl

#endif // !WL_SEAT_H

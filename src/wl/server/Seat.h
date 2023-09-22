// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_SEAT_H
#define WL_SERVER_SEAT_H

#include "Type.h"

#include <wayland-server-protocol.h>

#include <memory>

namespace wl {
namespace server {

class Resource;

class Seat : public Type<Seat, wl_seat>
{
    friend class Type;

public:
    Seat();
    ~Seat() override;

protected:
    virtual void wl_seat_get_pointer(Resource *resource, uint32_t id) = 0;
    virtual void wl_seat_get_keyboard(Resource *resource, uint32_t id) = 0;
    virtual void wl_seat_get_touch(Resource *resource, uint32_t id) = 0;
    virtual void wl_seat_release(Resource *resource) = 0;

private:
    static const struct wl_seat_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_SEAT_H

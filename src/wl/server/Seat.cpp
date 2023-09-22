// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Seat.h"

using namespace wl::server;

const struct wl_seat_interface Seat::impl = {
    ResourceCallbackWrapper<&Seat::wl_seat_get_pointer>::func,
    ResourceCallbackWrapper<&Seat::wl_seat_get_keyboard>::func,
    ResourceCallbackWrapper<&Seat::wl_seat_get_touch>::func,
    ResourceCallbackWrapper<&Seat::wl_seat_release>::func,
};

Seat::Seat()
    : Type()
{
}

Seat::~Seat() = default;

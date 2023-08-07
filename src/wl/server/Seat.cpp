// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Seat.h"

#include "utils.h"

using namespace wl::server;

template<>
const struct wl_seat_interface Seat::Type::impl = {
    ResourceCallbackWrapper<&Seat::getPointer>::func,
    ResourceCallbackWrapper<&Seat::getKeyboard>::func,
    ResourceCallbackWrapper<&Seat::getTouch>::func,
    ResourceCallbackWrapper<&Seat::release>::func,
};

Seat::Seat(wl_client *client, uint32_t id)
    : Type(client, id)
{
}

void Seat::getPointer([[maybe_unused]] struct wl_client *client, [[maybe_unused]] uint32_t id) { }

void Seat::getKeyboard([[maybe_unused]] struct wl_client *client, [[maybe_unused]] uint32_t id) { }

void Seat::getTouch([[maybe_unused]] struct wl_client *client, [[maybe_unused]] uint32_t id) { }

void Seat::release([[maybe_unused]] struct wl_client *client) { }

void Seat::resourceDestroy() { }

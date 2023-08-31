// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keyboard.h"

#include "Seat.h"

Keyboard::Keyboard(Seat *seat)
    : seat_(seat)
{
}

Keyboard::~Keyboard() { }

void Keyboard::sendKeymap(uint32_t format, int32_t fd, uint32_t size) {
}

void Keyboard::sendKey(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) { }

void Keyboard::wl_keyboard_release(wl::server::Resource *resource) {
    resource->destroy();
}

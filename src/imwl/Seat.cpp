// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Seat.h"

// #include "Keyboard.h"
#include "InputMethodV2.h"
#include "DimTextInputV1.h"
// #include "TextInputV3.h"
#include "VirtualKeyboardV1.h"

#include <QDebug>

Seat::Seat()
    : wl::server::Seat()
    // , keyboard_(std::make_shared<Keyboard>(this))
    , dimTextInputV1_(std::make_shared<DimTextInputV1>(this))
    // , textInputV3_(std::make_shared<TextInputV3>(this))
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
    // keyboard_->add(resource->client(), id);
}

void Seat::wl_seat_get_touch(wl::server::Resource *resource, uint32_t id)
{
    qWarning() << "wl_seat_get_touch";
}

void Seat::wl_seat_release(wl::server::Resource *resource)
{
    qWarning() << "wl_seat_release";
    resource->destroy();
}

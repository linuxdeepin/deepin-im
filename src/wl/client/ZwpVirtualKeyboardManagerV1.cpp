// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpVirtualKeyboardManagerV1.h"

#include "Seat.h"
#include "ZwpVirtualKeyboardV1.h"

using namespace wl::client;

ZwpVirtualKeyboardManagerV1::ZwpVirtualKeyboardManagerV1(zwp_virtual_keyboard_manager_v1 *val)
    : Type(val)
{
}

ZwpVirtualKeyboardManagerV1::~ZwpVirtualKeyboardManagerV1()
{
    zwp_virtual_keyboard_manager_v1_destroy(get());
}

std::shared_ptr<ZwpVirtualKeyboardV1>
ZwpVirtualKeyboardManagerV1::createVirtualKeyboard(const std::shared_ptr<Seat> &seat)
{
    return std::make_shared<ZwpVirtualKeyboardV1>(static_cast<zwp_virtual_keyboard_v1 *>(
        zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(get(), seat->get())));
}

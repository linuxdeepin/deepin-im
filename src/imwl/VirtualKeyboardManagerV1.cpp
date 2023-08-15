// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardManagerV1.h"

#include "Seat.h"
#include "VirtualKeyboardV1.h"

#include <QDebug>

VirtualKeyboardManagerV1::VirtualKeyboardManagerV1() { }

VirtualKeyboardManagerV1::~VirtualKeyboardManagerV1() { }

void VirtualKeyboardManagerV1::zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(
    wl::server::Resource *resource, struct ::wl_resource *seat, uint32_t id)
{
    qWarning() << "create virtual keyboard";
    auto *seat_ = dynamic_cast<Seat *>(wl::server::Seat::fromResource(seat));

    auto vk1 = seat_->getVirtualKeyboardV1();
    vk1->add(resource->client(), id);
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardManagerV1.h"

#include "Compositor.h"
#include "VirtualKeyboardV1.h"

#include <QDebug>
#include <QWaylandSeat>

VirtualKeyboardManagerV1::VirtualKeyboardManagerV1() { }

VirtualKeyboardManagerV1::~VirtualKeyboardManagerV1() { }

void VirtualKeyboardManagerV1::zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(
    wl::server::Resource *resource, struct ::wl_resource *seat, uint32_t id)
{
    qWarning() << "create virtual keyboard";
    auto *seat_ = QWaylandSeat::fromSeatResource(seat);

    auto vk1 = dynamic_cast<Compositor *>(seat_->compositor())->getVirtualKeyboardV1();
    vk1->add(resource->client(), id);
}

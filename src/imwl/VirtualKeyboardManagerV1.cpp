// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardManagerV1.h"

#include "VirtualKeyboardV1.h"
#include "wl/server/Seat.h"

#include <QDebug>

VirtualKeyboardManagerV1::VirtualKeyboardManagerV1() { }

VirtualKeyboardManagerV1::~VirtualKeyboardManagerV1() { }

void VirtualKeyboardManagerV1::zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(
    wl::server::Resource *resource, struct ::wl_resource *seat, uint32_t id)
{
    qWarning() << "create virtual keyboard";
    auto *s = wl::server::Seat::fromResource(seat);

    auto iter = virtualKeyboards_.find(seat);
    if (iter == virtualKeyboards_.end()) {
        auto *vk = new VirtualKeyboardV1(seat);
        auto [i, r] = virtualKeyboards_.emplace(seat, vk);
        iter = i;
    }

    iter->second->add(resource->client(), id);
}
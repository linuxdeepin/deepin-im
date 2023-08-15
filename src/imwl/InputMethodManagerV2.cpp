// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodManagerV2.h"

#include "InputMethodV2.h"
#include "Seat.h"
#include "wl/server/Seat.h"

InputMethodManagerV2::InputMethodManagerV2() { }

InputMethodManagerV2::~InputMethodManagerV2() { }

void InputMethodManagerV2::zwp_input_method_manager_v2_get_input_method(
    wl::server::Resource *resource, struct ::wl_resource *seat, uint32_t input_method)
{
    auto seat_ = dynamic_cast<Seat *>(wl::server::Seat::fromResource(seat));

    auto im2 = seat_->getInputMethodV2();
    im2->add(resource->client(), input_method);
}

void InputMethodManagerV2::zwp_input_method_manager_v2_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

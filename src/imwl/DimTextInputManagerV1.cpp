// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DimTextInputManagerV1.h"

#include "Seat.h"
#include "DimTextInputV1.h"
#include "wl/server/Seat.h"

DimTextInputManagerV1::DimTextInputManagerV1() { }

DimTextInputManagerV1::~DimTextInputManagerV1() { }

void DimTextInputManagerV1::zwp_dim_text_input_manager_v1_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

void DimTextInputManagerV1::zwp_dim_text_input_manager_v1_get_text_input(wl::server::Resource *resource,
                                                                  uint32_t id,
                                                                  struct ::wl_resource *seat)
{
    auto seat_ = dynamic_cast<Seat *>(wl::server::Seat::fromResource(seat));

    auto ti3 = seat_->getDimTextInputV1();
    ti3->add(resource->client(), id);
}

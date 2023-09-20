// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DimTextInputManagerV1.h"

#include "Compositor.h"
#include "DimTextInputV1.h"

DimTextInputManagerV1::DimTextInputManagerV1() { }

DimTextInputManagerV1::~DimTextInputManagerV1() { }

void DimTextInputManagerV1::zwp_dim_text_input_manager_v1_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

void DimTextInputManagerV1::zwp_dim_text_input_manager_v1_get_text_input(
    wl::server::Resource *resource, uint32_t id, struct ::wl_resource *seat)
{
    auto seat_ = QWaylandSeat::fromSeatResource(seat);

    auto dti1 = dynamic_cast<Compositor *>(seat_->compositor())->getDimTextInputV1();
    dti1->add(resource->client(), id);
}

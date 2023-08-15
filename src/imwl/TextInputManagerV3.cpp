// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TextInputManagerV3.h"

#include "Seat.h"
#include "TextInputV3.h"
#include "wl/server/Seat.h"

TextInputManagerV3::TextInputManagerV3() { }

TextInputManagerV3::~TextInputManagerV3() { }

void TextInputManagerV3::zwp_text_input_manager_v3_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

void TextInputManagerV3::zwp_text_input_manager_v3_get_text_input(wl::server::Resource *resource,
                                                                  uint32_t id,
                                                                  struct ::wl_resource *seat)
{
    auto seat_ = dynamic_cast<Seat *>(wl::server::Seat::fromResource(seat));

    auto ti3 = seat_->getTextInputV3();
    ti3->add(resource->client(), id);
}

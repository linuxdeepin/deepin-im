// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TextInputManagerV3.h"

#include "TextInputV3.h"
#include "wl/server/Seat.h"

TextInputManagerV3::TextInputManagerV3(Core *core)
    : core_(core)
{
}

TextInputManagerV3::~TextInputManagerV3() { }

TextInputV3 *TextInputManagerV3::getTextInputV4BySeat(wl::server::Seat *seat)
{
    return textInputs_.find(seat) != textInputs_.end() ? textInputs_.at(seat) : nullptr;
}

void TextInputManagerV3::zwp_text_input_manager_v3_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

void TextInputManagerV3::zwp_text_input_manager_v3_get_text_input(wl::server::Resource *resource,
                                                                  uint32_t id,
                                                                  struct ::wl_resource *seat)
{
    auto seat_ = wl::server::Seat::fromResource(seat);
    auto iter = textInputs_.find(seat_);
    if (iter == textInputs_.end()) {
        auto *ti = new TextInputV3(core_, seat_);
        auto [i, r] = textInputs_.emplace(seat_, ti);
        iter = i;
    }

    iter->second->add(resource->client(), id);
}

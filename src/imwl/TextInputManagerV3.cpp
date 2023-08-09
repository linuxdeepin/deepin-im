// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TextInputManagerV3.h"

#include "TextInputV3.h"

TextInputManagerV3::TextInputManagerV3(Core *core)
    : core_(core)
{
}

TextInputManagerV3::~TextInputManagerV3() { }

TextInputV3 *TextInputManagerV3::getTextInputV4BySeat(struct ::wl_resource *seat)
{
    return textInputs_.at(seat);
}

void TextInputManagerV3::zwp_text_input_manager_v3_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

void TextInputManagerV3::zwp_text_input_manager_v3_get_text_input(wl::server::Resource *resource,
                                                                  uint32_t id,
                                                                  struct ::wl_resource *seat)
{
    auto iter = textInputs_.find(seat);
    if (iter == textInputs_.end()) {
        auto *ti = new TextInputV3(core_, seat);
        auto [i, r] = textInputs_.emplace(seat, ti);
        iter = i;
    }

    iter->second->add(resource->client(), id);
}
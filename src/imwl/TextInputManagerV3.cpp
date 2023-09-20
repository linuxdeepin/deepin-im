// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TextInputManagerV3.h"

#include "Compositor.h"
#include "TextInputV3.h"

#include <QtWaylandCompositor/QWaylandSeat>

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
    auto *seat_ = QWaylandSeat::fromSeatResource(seat);

    auto ti3 = dynamic_cast<Compositor *>(seat_->compositor())->getTextInputV3();
    ti3->add(resource->client(), id);
}

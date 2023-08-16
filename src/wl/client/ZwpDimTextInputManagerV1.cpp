// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpDimTextInputManagerV1.h"

#include "Seat.h"
#include "ZwpDimTextInputV1.h"

using namespace wl::client;

ZwpDimTextInputManagerV1::ZwpDimTextInputManagerV1(zwp_dim_text_input_manager_v1 *val)
    : Type(val)
{
}

ZwpDimTextInputManagerV1::~ZwpDimTextInputManagerV1()
{
    zwp_dim_text_input_manager_v1_destroy(get());
}

std::shared_ptr<ZwpDimTextInputV1>
ZwpDimTextInputManagerV1::getTextInput(const std::shared_ptr<Seat> &seat)
{
    return std::make_shared<ZwpDimTextInputV1>(
        zwp_dim_text_input_manager_v1_get_text_input(get(), seat->get()));
}

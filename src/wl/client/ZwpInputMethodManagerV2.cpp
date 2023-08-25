// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZwpInputMethodManagerV2.h"

#include "Seat.h"
#include "ZwpInputMethodV2.h"

using namespace wl::client;

ZwpInputMethodManagerV2::ZwpInputMethodManagerV2(zwp_input_method_manager_v2 *val)
    : Type(val)
{
}

ZwpInputMethodManagerV2::~ZwpInputMethodManagerV2()
{
    zwp_input_method_manager_v2_destroy(get());
}

std::shared_ptr<ZwpInputMethodV2>
ZwpInputMethodManagerV2::get_input_method(const std::shared_ptr<Seat> &seat)
{
    return std::make_shared<ZwpInputMethodV2>(static_cast<zwp_input_method_v2 *>(
        zwp_input_method_manager_v2_get_input_method(get(), seat->get())));
}

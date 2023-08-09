// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodManagerV2.h"

#include "InputMethodV2.h"

InputMethodManagerV2::InputMethodManagerV2(Core *core)
    : core_(core)
{
}

InputMethodManagerV2::~InputMethodManagerV2() { }

std::shared_ptr<InputMethodV2>
InputMethodManagerV2::getInputMethodV2BySeat(struct ::wl_resource *seat)
{
    return inputmethods_.at(seat);
}

void InputMethodManagerV2::zwp_input_method_manager_v2_get_input_method(
    wl::server::Resource *resource, struct ::wl_resource *seat, uint32_t input_method)
{
    auto iter = inputmethods_.find(seat);
    if (iter == inputmethods_.end()) {
        auto im = std::make_shared<InputMethodV2>(core_, seat);
        auto [i, r] = inputmethods_.emplace(seat, im);
        iter = i;
    }

    iter->second->add(resource->client(), input_method);
}

void InputMethodManagerV2::zwp_input_method_manager_v2_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}
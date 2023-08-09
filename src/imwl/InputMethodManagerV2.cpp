// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodManagerV2.h"

#include "InputMethodV2.h"
#include "common.h"
#include "wl/server/ZwpInputMethodManagerV2.h"

class InputMethodManagerV2Private : public wl::server::ZwpInputMethodManagerV2
{
public:
    InputMethodManagerV2Private(InputMethodManagerV2 *q)
        : wl::server::ZwpInputMethodManagerV2()
        , q(q)
    {
    }

    ~InputMethodManagerV2Private() { }

protected:
    void zwp_input_method_manager_v2_get_input_method(wl::server::Resource *resource,
                                                      struct ::wl_resource *seat,
                                                      uint32_t input_method) override
    {
        auto iter = q->inputmethods_.find(seat);
        if (iter == q->inputmethods_.end()) {
            auto *im = new InputMethodV2(q->core_, seat, q);
            auto [i, r] = q->inputmethods_.emplace(seat, im);
            iter = i;
        }

        iter->second->add(resource->client(), input_method);
    }

    void zwp_input_method_manager_v2_destroy(wl::server::Resource *resource) override {
        resource->destroy();
    }

private:
    InputMethodManagerV2 *q;
};

InputMethodManagerV2::InputMethodManagerV2(Core *core)
    : core_(core)
    , d(new InputMethodManagerV2Private(this))
{
}

InputMethodManagerV2::~InputMethodManagerV2() { }

INIT_FUNCS(InputMethodManagerV2)

InputMethodV2 *InputMethodManagerV2::getInputMethodV2BySeat(struct ::wl_resource *seat)
{
    return inputmethods_.at(seat);
}

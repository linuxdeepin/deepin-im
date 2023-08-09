// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodV2.h"

#include "Core.h"
#include "InputMethodKeyboardGrabV2.h"
#include "TextInputManagerV3.h"
#include "TextInputV3.h"
#include "common.h"
#include "wl/server/ZwpInputMethodV2.h"

class InputMethodV2Private : public wl::server::ZwpInputMethodV2
{
public:
    InputMethodV2Private(InputMethodV2 *q)
        : q(q)
    {
    }

    ~InputMethodV2Private() { }

protected:
    void zwp_input_method_v2_commit_string(wl::server::Resource *resource,
                                           const char *text) override
    {
        auto *im = getTextInputV3();
        im->sendCommitString(text);
    }

    void zwp_input_method_v2_set_preedit_string(wl::server::Resource *resource,
                                                const char *text,
                                                int32_t cursor_begin,
                                                int32_t cursor_end) override
    {
        auto *im = getTextInputV3();
        im->sendPreeditString(text, cursor_begin, cursor_end);
    }

    void zwp_input_method_v2_delete_surrounding_text(wl::server::Resource *resource,
                                                     uint32_t before_length,
                                                     uint32_t after_length) override
    {
    }

    void zwp_input_method_v2_commit(wl::server::Resource *resource, uint32_t serial) override
    {
        auto *im = getTextInputV3();
        im->sendDone(serial);
    }

    void zwp_input_method_v2_get_input_popup_surface(wl::server::Resource *resource,
                                                     uint32_t id,
                                                     struct ::wl_resource *surface) override
    {
    }

    void zwp_input_method_v2_grab_keyboard(wl::server::Resource *resource,
                                           uint32_t keyboard) override
    {
        q->grab_->add(resource->client(), keyboard);
    }

    void zwp_input_method_v2_destroy(wl::server::Resource *resource) override
    {
        resource->destroy();
    }

private:
    InputMethodV2 *q;

    TextInputV3 *getTextInputV3()
    {
        return q->core_->getTextInputManagerV3()->getTextInputV4BySeat(q->seat_);
    }
};

InputMethodV2::InputMethodV2(Core *core, struct ::wl_resource *seat)
    : d(new InputMethodV2Private(this))
    , core_(core)
    , seat_(seat)
    , grab_(std::make_unique<InputMethodKeyboardGrabV2>(seat))
{
}

InputMethodV2::~InputMethodV2() { }

INIT_FUNCS(InputMethodV2)

void InputMethodV2::sendDeactivate()
{
    // const auto resources = d->resourceMap();
    // for (auto *resource : resources) {
    //     d->send_deactivate(resource->handle);
    // }
}

void InputMethodV2::sendActivate()
{
    // const auto resources = d->resourceMap();
    // for (auto *resource : resources) {
    //     d->send_activate(resource->handle);
    // }
}

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodV2.h"

#include "Seat.h"
#include "InputMethodKeyboardGrabV2.h"
#include "TextInputV3.h"

InputMethodV2::InputMethodV2(Seat *seat)
    : seat_(seat)
    , grab_(std::make_unique<InputMethodKeyboardGrabV2>(seat))
{
}

InputMethodV2::~InputMethodV2() { }

void InputMethodV2::sendDeactivate()
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        send_deactivate(resource->handle);
    }
}

void InputMethodV2::sendActivate()
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        send_activate(resource->handle);
    }
}

void InputMethodV2::zwp_input_method_v2_commit_string(wl::server::Resource *resource,
                                                      const char *text)
{
    auto ti3 = seat_->getTextInputV3();
    if (ti3) {
        ti3->sendCommitString(text);
    }
}

void InputMethodV2::zwp_input_method_v2_set_preedit_string(wl::server::Resource *resource,
                                                           const char *text,
                                                           int32_t cursor_begin,
                                                           int32_t cursor_end)
{
    auto ti3 = seat_->getTextInputV3();
    if (ti3) {
        ti3->sendPreeditString(text, cursor_begin, cursor_end);
    }
}

void InputMethodV2::zwp_input_method_v2_delete_surrounding_text(wl::server::Resource *resource,
                                                                uint32_t before_length,
                                                                uint32_t after_length)
{
}

void InputMethodV2::zwp_input_method_v2_commit(wl::server::Resource *resource, uint32_t serial)
{
    auto ti3 = seat_->getTextInputV3();
    if (ti3) {
        ti3->sendDone(serial);
    }
}

void InputMethodV2::zwp_input_method_v2_get_input_popup_surface(wl::server::Resource *resource,
                                                                uint32_t id,
                                                                struct ::wl_resource *surface)
{
}

void InputMethodV2::zwp_input_method_v2_grab_keyboard(wl::server::Resource *resource,
                                                      uint32_t keyboard)
{
    grab_->add(resource->client(), keyboard);
}

void InputMethodV2::zwp_input_method_v2_destroy(wl::server::Resource *resource)
{
    resource->destroy();
}

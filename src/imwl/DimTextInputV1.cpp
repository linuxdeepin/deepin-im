// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DimTextInputV1.h"

#include "InputMethodV2.h"
#include "Seat.h"
#include "wl/server/Resource.h"

DimTextInputV1::DimTextInputV1(Seat *seat)
    : seat_(seat)
{
    constexpr char data[] = "Shift\0Lock\0Control\0Mod1\0Mod2\0Mod3\0Mod4";

    wl_array_init(&modifiersMap_);
    auto *ptr = wl_array_add(&modifiersMap_, sizeof(data));
    std::copy_n(data, sizeof(data), static_cast<char *>(ptr));
}

DimTextInputV1::~DimTextInputV1()
{
    wl_array_release(&modifiersMap_);
}

void DimTextInputV1::sendPreeditString(const char *text, int32_t cursor_begin, int32_t cursor_end)
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        if (m_enabled.find(resource.get()) == m_enabled.end()) {
            continue;
        }
        send_preedit_string(resource->handle, text, cursor_begin, cursor_end);
    }
}

void DimTextInputV1::sendCommitString(const char *text)
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        if (m_enabled.find(resource.get()) == m_enabled.end()) {
            continue;
        }
        send_commit_string(resource->handle, text);
    }
}

void DimTextInputV1::sendDone(uint32_t serial)
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        if (m_enabled.find(resource.get()) == m_enabled.end()) {
            continue;
        }
        send_done(resource->handle, serial);
    }
}

void DimTextInputV1::sendKeysym(
    uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers)
{
    const auto resources = resourceMap();
    for (auto &[client, resource] : resources) {
        if (m_enabled.find(resource.get()) == m_enabled.end()) {
            continue;
        }
        send_keysym(resource->handle, serial, time, sym, state, modifiers);
    }
}

void DimTextInputV1::resource_bind(wl::server::Resource *resource)
{
    send_modifiers_map(resource->handle, &modifiersMap_);
}

void DimTextInputV1::zwp_dim_text_input_v1_destroy(wl::server::Resource *resource)
{
    m_enabled.erase(resource);

    resource->destroy();
}

void DimTextInputV1::zwp_dim_text_input_v1_enable(wl::server::Resource *resource)
{
    m_enabled.emplace(resource);

    auto im2 = seat_->getInputMethodV2();
    im2->sendActivate();
}

void DimTextInputV1::zwp_dim_text_input_v1_disable(wl::server::Resource *resource)
{
    m_enabled.erase(resource);

    auto im2 = seat_->getInputMethodV2();
    im2->sendDeactivate();
}

void DimTextInputV1::zwp_dim_text_input_v1_set_surrounding_text(wl::server::Resource *resource,
                                                                const char *text,
                                                                int32_t cursor,
                                                                int32_t anchor)
{
}

void DimTextInputV1::zwp_dim_text_input_v1_set_text_change_cause(wl::server::Resource *resource,
                                                                 uint32_t cause)
{
}

void DimTextInputV1::zwp_dim_text_input_v1_set_content_type(wl::server::Resource *resource,
                                                            uint32_t hint,
                                                            uint32_t purpose)
{
}

void DimTextInputV1::zwp_dim_text_input_v1_set_cursor_rectangle(
    wl::server::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height)
{
}

void DimTextInputV1::zwp_dim_text_input_v1_commit(wl::server::Resource *resource) { }

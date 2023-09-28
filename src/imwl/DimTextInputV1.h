// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIMTEXTINPUTV3_H
#define DIMTEXTINPUTV3_H

#include "wl/server/ZwpDimTextInputV1.h"

#include <set>

class Seat;

class DimTextInputV1 : public wl::server::ZwpDimTextInputV1
{
public:
    explicit DimTextInputV1(Seat *seat);
    ~DimTextInputV1() override;

    void enterPid(pid_t pid);
    void leavePid(pid_t pid);

    void sendPreeditString(const char *text, int32_t cursor_begin, int32_t cursor_end);
    void sendCommitString(const char *text);
    void sendDone(uint32_t serial);
    void sendKeysym(uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers);

protected:
    void resource_bind(wl::server::Resource *resource) override;

    void zwp_dim_text_input_v1_destroy(wl::server::Resource *resource) override;
    void zwp_dim_text_input_v1_enable(wl::server::Resource *resource) override;
    void zwp_dim_text_input_v1_disable(wl::server::Resource *resource) override;
    void zwp_dim_text_input_v1_set_surrounding_text(wl::server::Resource *resource,
                                                const char *text,
                                                int32_t cursor,
                                                int32_t anchor) override;
    void zwp_dim_text_input_v1_set_text_change_cause(wl::server::Resource *resource,
                                                 uint32_t cause) override;
    void zwp_dim_text_input_v1_set_content_type(wl::server::Resource *resource,
                                            uint32_t hint,
                                            uint32_t purpose) override;
    void zwp_dim_text_input_v1_set_cursor_rectangle(wl::server::Resource *resource,
                                                int32_t x,
                                                int32_t y,
                                                int32_t width,
                                                int32_t height) override;
    void zwp_dim_text_input_v1_commit(wl::server::Resource *resource) override;

private:
    Seat *seat_;
    std::unordered_map<pid_t, wl::server::Resource *> pidMap_;
    pid_t enteredPid_;
    std::set<wl::server::Resource *> m_enabled;
    struct wl_array modifiersMap_;
};

#endif // !DIMTEXTINPUTV3_H

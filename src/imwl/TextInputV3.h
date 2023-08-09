// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TEXTINPUTV3_H
#define TEXTINPUTV3_H

#include "wl/server/ZwpTextInputV3.h"

#include <set>

class Core;

class TextInputV3 : public wl::server::ZwpTextInputV3
{
public:
    TextInputV3(Core *core, struct ::wl_resource *seat);
    ~TextInputV3();

    void sendPreeditString(const char *text, int32_t cursor_begin, int32_t cursor_end);
    void sendCommitString(const char *text);
    void sendDone(uint32_t serial);

protected:
    void zwp_text_input_v3_destroy(wl::server::Resource *resource) override;
    void zwp_text_input_v3_enable(wl::server::Resource *resource) override;
    void zwp_text_input_v3_disable(wl::server::Resource *resource) override;
    void zwp_text_input_v3_set_surrounding_text(wl::server::Resource *resource,
                                                const char *text,
                                                int32_t cursor,
                                                int32_t anchor) override;
    void zwp_text_input_v3_set_text_change_cause(wl::server::Resource *resource,
                                                 uint32_t cause) override;
    void zwp_text_input_v3_set_content_type(wl::server::Resource *resource,
                                            uint32_t hint,
                                            uint32_t purpose) override;
    void zwp_text_input_v3_set_cursor_rectangle(wl::server::Resource *resource,
                                                int32_t x,
                                                int32_t y,
                                                int32_t width,
                                                int32_t height) override;
    void zwp_text_input_v3_commit(wl::server::Resource *resource) override;

private:
    Core *core_;
    struct ::wl_resource *seat_;
    std::set<wl::server::Resource *> m_enabled;
};

#endif // !TEXTINPUTV3_H

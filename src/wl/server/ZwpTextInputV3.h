// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPTEXTINPUTV3_H
#define WL_SERVER_ZWPTEXTINPUTV3_H

#include "Type.h"
#include "wayland-text-input-unstable-v3-server-protocol.h"

namespace wl {
namespace server {

class ZwpTextInputV3 : public Type<ZwpTextInputV3, zwp_text_input_v3>
{
    friend class Type;

public:
    ZwpTextInputV3();

    void send_enter(struct wl_resource *resource, struct wl_resource *surface);
    void send_leave(struct wl_resource *resource, struct wl_resource *surface);
    void send_preedit_string(struct wl_resource *resource, const char *text, int32_t cursor_begin, int32_t cursor_end);
    void send_commit_string(struct wl_resource *resource, const char *text);
    void send_delete_surrounding_text(struct wl_resource *resource, uint32_t before_length, uint32_t after_length);
    void send_done(struct wl_resource *resource, uint32_t serial);

protected:
    virtual void zwp_text_input_v3_destroy(Resource *resource) = 0;
    virtual void zwp_text_input_v3_enable(Resource *resource) = 0;
    virtual void zwp_text_input_v3_disable(Resource *resource) = 0;
    virtual void zwp_text_input_v3_set_surrounding_text(Resource *resource, const char *text, int32_t cursor, int32_t anchor) = 0;
    virtual void zwp_text_input_v3_set_text_change_cause(Resource *resource, uint32_t cause) = 0;
    virtual void zwp_text_input_v3_set_content_type(Resource *resource, uint32_t hint, uint32_t purpose) = 0;
    virtual void zwp_text_input_v3_set_cursor_rectangle(Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) = 0;
    virtual void zwp_text_input_v3_commit(Resource *resource) = 0;

private:
    static const struct zwp_text_input_v3_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPTEXTINPUTV3_H

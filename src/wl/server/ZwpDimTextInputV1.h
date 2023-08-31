// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPDIMTEXTINPUTV1_H
#define WL_SERVER_ZWPDIMTEXTINPUTV1_H

#include "Type.h"
#include "wayland-dim-text-input-unstable-v1-server-protocol.h"

namespace wl {
namespace server {

class ZwpDimTextInputV1 : public Type<ZwpDimTextInputV1, zwp_dim_text_input_v1>
{
    friend class Type;

public:
    ZwpDimTextInputV1();

    void send_enter(struct wl_resource *resource);
    void send_leave(struct wl_resource *resource);
    void send_modifiers_map(struct wl_resource *resource, struct wl_array *map);
    void send_preedit_string(struct wl_resource *resource, const char *text, int32_t cursor_begin, int32_t cursor_end);
    void send_commit_string(struct wl_resource *resource, const char *text);
    void send_delete_surrounding_text(struct wl_resource *resource, uint32_t before_length, uint32_t after_length);
    void send_done(struct wl_resource *resource, uint32_t serial);
    void send_keysym(struct wl_resource *resource, uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers);

protected:
    virtual void zwp_dim_text_input_v1_destroy(Resource *resource) = 0;
    virtual void zwp_dim_text_input_v1_enable(Resource *resource) = 0;
    virtual void zwp_dim_text_input_v1_disable(Resource *resource) = 0;
    virtual void zwp_dim_text_input_v1_set_surrounding_text(Resource *resource, const char *text, int32_t cursor, int32_t anchor) = 0;
    virtual void zwp_dim_text_input_v1_set_text_change_cause(Resource *resource, uint32_t cause) = 0;
    virtual void zwp_dim_text_input_v1_set_content_type(Resource *resource, uint32_t hint, uint32_t purpose) = 0;
    virtual void zwp_dim_text_input_v1_set_cursor_rectangle(Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) = 0;
    virtual void zwp_dim_text_input_v1_commit(Resource *resource) = 0;

private:
    static const struct zwp_dim_text_input_v1_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPDIMTEXTINPUTV1_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_ZWPDIMTEXTINPUTV3_H
#define WL_CLIENT_ZWPDIMTEXTINPUTV3_H

#include "Type.h"
#include "wayland-dim-text-input-unstable-v1-client-protocol.h"

namespace wl {
namespace client {

class ZwpDimTextInputV1 : public Type<zwp_dim_text_input_v1>
{
public:
    ZwpDimTextInputV1(zwp_dim_text_input_v1 *val);
    virtual ~ZwpDimTextInputV1();

    void enable();
    void disable();
    void set_surrounding_text(const char *text, int32_t cursor, int32_t anchor);
    void set_text_change_cause(uint32_t cause);
    void set_content_type(uint32_t hint, uint32_t purpose);
    void set_cursor_rectangle(int32_t x, int32_t y, int32_t width, int32_t height);
    void commit();

protected:
    virtual void zwp_dim_text_input_v1_modifiers_map(struct wl_array *map) = 0;
    virtual void zwp_dim_text_input_v1_preedit_string(const char *text, int32_t cursor_begin, int32_t cursor_end) = 0;
    virtual void zwp_dim_text_input_v1_commit_string(const char *text) = 0;
    virtual void zwp_dim_text_input_v1_delete_surrounding_text(uint32_t before_length, uint32_t after_length) = 0;
    virtual void zwp_dim_text_input_v1_done(uint32_t serial) = 0;
    virtual void zwp_dim_text_input_v1_keysym(uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers) = 0;

private:
    static const zwp_dim_text_input_v1_listener listener_;
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_ZWPDIMTEXTINPUTV3_H

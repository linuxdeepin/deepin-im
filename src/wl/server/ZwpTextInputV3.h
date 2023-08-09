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

    void destroy(struct wl_client *client);
    void enable(struct wl_client *client);
    void disable(struct wl_client *client);
    void
    setSurroundingText(struct wl_client *client, const char *text, int32_t cursor, int32_t anchor);
    void setTextChangeCause(struct wl_client *client, uint32_t cause);
    void setContentType(struct wl_client *client, uint32_t hint, uint32_t purpose);
    void setCursorRectangle(
        struct wl_client *client, int32_t x, int32_t y, int32_t width, int32_t height);
    void commit(struct wl_client *client);

protected:
    virtual void zwp_text_input_v3_destroy(wl::server::Resource *resource) = 0;
    virtual void zwp_text_input_v3_enable(wl::server::Resource *resource) = 0;
    virtual void zwp_text_input_v3_disable(wl::server::Resource *resource) = 0;
    virtual void zwp_text_input_v3_set_surrounding_text(wl::server::Resource *resource, const char *text, int32_t cursor, int32_t anchor) = 0;
    virtual void zwp_text_input_v3_set_text_change_cause(wl::server::Resource *resource, uint32_t cause) = 0;
    virtual void zwp_text_input_v3_set_content_type(wl::server::Resource *resource, uint32_t hint, uint32_t purpose) = 0;
    virtual void zwp_text_input_v3_set_cursor_rectangle(wl::server::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) = 0;
    virtual void zwp_text_input_v3_commit(wl::server::Resource *resource) = 0;

private:
    static const struct zwp_text_input_v3_interface impl;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPTEXTINPUTV3_H

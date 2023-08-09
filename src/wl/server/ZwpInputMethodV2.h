// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPINPUTMETHODV2_H
#define WL_SERVER_ZWPINPUTMETHODV2_H

#include "Type.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"

#include <memory>

namespace wl {
namespace server {

class ZwpInputMethodV2 : public Type<ZwpInputMethodV2, zwp_input_method_v2>
{
    friend class Type;

public:
    ZwpInputMethodV2();

protected:
    virtual void zwp_input_method_v2_commit_string(wl::server::Resource *resource, const char *text) = 0;
    virtual void zwp_input_method_v2_set_preedit_string(wl::server::Resource *resource, const char *text, int32_t cursor_begin, int32_t cursor_end) = 0;
    virtual void zwp_input_method_v2_delete_surrounding_text(wl::server::Resource *resource, uint32_t before_length, uint32_t after_length) = 0;
    virtual void zwp_input_method_v2_commit(wl::server::Resource *resource, uint32_t serial) = 0;
    virtual void zwp_input_method_v2_get_input_popup_surface(wl::server::Resource *resource, uint32_t id, struct ::wl_resource *surface) = 0;
    virtual void zwp_input_method_v2_grab_keyboard(wl::server::Resource *resource, uint32_t keyboard) = 0;
    virtual void zwp_input_method_v2_destroy(wl::server::Resource *resource) = 0;

private:
    static const struct zwp_input_method_v2_interface impl;

    void commitString(struct wl_client *client, const char *text);
    void setPreeditString(struct wl_client *client,
                          const char *text,
                          int32_t cursor_begin,
                          int32_t cursor_end);
    void deleteSurroundingText(struct wl_client *client,
                               uint32_t before_length,
                               uint32_t after_length);
    void commit(struct wl_client *client, uint32_t serial);
    void getInputPopupSurface(struct wl_client *client, uint32_t id, struct wl_resource *surface);
    void grabKeyboard(struct wl_client *client, uint32_t keyboard);
    void destroy(struct wl_client *client);
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODV2_H

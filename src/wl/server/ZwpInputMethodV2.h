#ifndef WL_SERVER_ZWPINPUTMETHODV2_H
#define WL_SERVER_ZWPINPUTMETHODV2_H

#include "wayland-input-method-unstable-v2-server-protocol.h"
#include "wl/Type.h"

#include <memory>

namespace wl {

class ZwpInputMethodV2 : public Type<zwp_input_method_v2>
{
public:
    static const struct zwp_input_method_v2_interface impl;

    void commitString(struct wl_client *client, struct wl_resource *resource, const char *text);
    void setPreeditString(struct wl_client *client,
                          struct wl_resource *resource,
                          const char *text,
                          int32_t cursor_begin,
                          int32_t cursor_end);
    void deleteSurroundingText(struct wl_client *client,
                               struct wl_resource *resource,
                               uint32_t before_length,
                               uint32_t after_length);
    void commit(struct wl_client *client, struct wl_resource *resource, uint32_t serial);
    void getInputPopupSurface(struct wl_client *client,
                              struct wl_resource *resource,
                              uint32_t id,
                              struct wl_resource *surface);
    void grabKeyboard(struct wl_client *client, struct wl_resource *resource, uint32_t keyboard);
    void destroy(struct wl_client *client, struct wl_resource *resource);
};

} // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODV2_H

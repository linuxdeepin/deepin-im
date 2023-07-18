#ifndef WL_SERVER_ZWPINPUTMETHODMANAGERV2_H
#define WL_SERVER_ZWPINPUTMETHODMANAGERV2_H

#include "wayland-input-method-unstable-v2-server-protocol.h"
#include "wl/Type.h"

#include <memory>

namespace wl {

class Seat;
class ZwpInputMethodV2;

class ZwpInputMethodManagerV2 : public Type<zwp_input_method_manager_v2>
{
public:
    static const struct zwp_input_method_manager_v2_interface impl;

    static void getInputMethod(struct wl_client *client,
                        struct wl_resource *resource,
                        struct wl_resource *seat,
                        uint32_t input_method);
    static void destroy(struct wl_client *client, struct wl_resource *resource);
};

}; // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODMANAGERV2_H

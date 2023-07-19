#ifndef WL_SERVER_ZWPINPUTMETHODMANAGERV2_H
#define WL_SERVER_ZWPINPUTMETHODMANAGERV2_H

#include "Type.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"

#include <memory>

namespace wl {
namespace server {

class Resource;
class Seat;
class ZwpInputMethodV2;

class ZwpInputMethodManagerV2 : public BASE_TYPE(zwp_input_method_manager_v2)
{
    friend class Type;

public:
    ZwpInputMethodManagerV2(wl_client *client, uint32_t id);

private:
    void getInputMethod(struct wl_client *client, struct wl_resource *seat, uint32_t input_method);
    void destroy(struct wl_client *client);

    void resourceDestroy() override;
};

} // namespace server
}; // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODMANAGERV2_H

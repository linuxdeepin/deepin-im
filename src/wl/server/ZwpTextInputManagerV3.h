#ifndef WL_SERVER_ZWPTEXTINPUTMANAGERV3_H
#define WL_SERVER_ZWPTEXTINPUTMANAGERV3_H

#include "Type.h"
#include "wayland-text-input-unstable-v3-server-protocol.h"

namespace wl {
namespace server {

class ZwpTextInputManagerV3 : public BASE_TYPE(zwp_text_input_manager_v3)
{
    friend class Type;

public:
    ZwpTextInputManagerV3(struct wl_client *client, uint32_t id);

private:
    void destroy(struct wl_client *client);
    void getTextInput(struct wl_client *client, uint32_t id, struct wl_resource *seat);

    void resourceDestroy() override;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPTEXTINPUTMANAGERV3_H

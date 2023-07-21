#ifndef WL_SERVER_ZWPVIRTUALKEYBOARDMANAGERV1_H
#define WL_SERVER_ZWPVIRTUALKEYBOARDMANAGERV1_H

#include "Type.h"
#include "wayland-virtual-keyboard-unstable-v1-server-protocol.h"

#include <memory>

namespace wl {
namespace server {

class ZwpVirtualKeyboardManagerV1 : public BASE_TYPE(zwp_virtual_keyboard_manager_v1)
{
    friend class Type;

public:
    ZwpVirtualKeyboardManagerV1(struct wl_client *client, uint32_t id);
    ~ZwpVirtualKeyboardManagerV1();

private:
    void createVirtualKeyboard(struct wl_client *client, struct wl_resource *seat, uint32_t id);

    void resourceDestroy() override;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPVIRTUALKEYBOARDMANAGERV1_H

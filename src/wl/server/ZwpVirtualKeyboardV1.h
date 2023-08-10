#ifndef ZWPVIRTUALKEYBOARDV1_H
#define ZWPVIRTUALKEYBOARDV1_H

#include "Type.h"
#include "wayland-virtual-keyboard-unstable-v1-server-protocol.h"

namespace wl {
namespace server {

class ZwpVirtualKeyboardV1 : public Type<ZwpVirtualKeyboardV1, zwp_virtual_keyboard_v1>
{
    friend class Type;

public:
    ZwpVirtualKeyboardV1();
    virtual ~ZwpVirtualKeyboardV1() = default;

    virtual void zwp_virtual_keyboard_v1_keymap(Resource *resource, uint32_t format, int32_t fd, uint32_t size) = 0;
    virtual void zwp_virtual_keyboard_v1_key(Resource *resource, uint32_t time, uint32_t key, uint32_t state) = 0;
    virtual void zwp_virtual_keyboard_v1_modifiers(Resource *resource, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) = 0;
    virtual void zwp_virtual_keyboard_v1_destroy(Resource *resource) = 0;

private:
    static const struct zwp_virtual_keyboard_v1_interface impl;
};

} // namespace server
} // namespace wl

#endif // !ZWPVIRTUALKEYBOARDV1_H

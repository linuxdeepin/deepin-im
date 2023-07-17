#ifndef WL_ZWPVIRTUALKEYBOARDV1_H
#define WL_ZWPVIRTUALKEYBOARDV1_H

#include "Type.h"

namespace wl {

class ZwpVirtualKeyboardV1 : public Type<zwp_virtual_keyboard_v1>
{
public:
    ZwpVirtualKeyboardV1(zwp_virtual_keyboard_v1 *val);
    ~ZwpVirtualKeyboardV1();

    void keymap(uint32_t format, int32_t fd, uint32_t size);
    void key(uint32_t time, uint32_t key, uint32_t state);
    void
    modifiers(uint32_t modsDepressed, uint32_t modsLatched, uint32_t modsLocked, uint32_t group);
};

} // namespace wl

#endif // !WL_ZWPVIRTUALKEYBOARDV1_H

#ifndef WL_SERVER_ZWPINPUTMETHODKEYBOARDGRABV2_H
#define WL_SERVER_ZWPINPUTMETHODKEYBOARDGRABV2_H

#include "Type.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"

namespace wl {
namespace server {

class ZwpInputMethodKeyboardGrabV2
    : public Type<ZwpInputMethodKeyboardGrabV2, zwp_input_method_keyboard_grab_v2>
{
    friend class Type;

public:
    ZwpInputMethodKeyboardGrabV2();
    virtual ~ZwpInputMethodKeyboardGrabV2();

protected:
    virtual void zwp_input_method_keyboard_grab_v2_release(Resource *resource) = 0;

private:
    static const struct zwp_input_method_keyboard_grab_v2_interface impl;

};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODKEYBOARDGRABV2_H

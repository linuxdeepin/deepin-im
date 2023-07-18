#ifndef WL_ZWPINPUTMETHODKEYBOARDGRABV2_H
#define WL_ZWPINPUTMETHODKEYBOARDGRABV2_H

#include "wl/Type.h"

namespace wl {

class ZwpInputMethodKeyboardGrabV2 : public Type<zwp_input_method_keyboard_grab_v2>
{
public:
    ZwpInputMethodKeyboardGrabV2(zwp_input_method_keyboard_grab_v2 *val);
    ~ZwpInputMethodKeyboardGrabV2();
};

}; // namespace wl

#endif // !WL_ZWPINPUTMETHODKEYBOARDGRABV2_H

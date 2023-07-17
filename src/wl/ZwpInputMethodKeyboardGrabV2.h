#ifndef ZWPINPUTMETHODKEYBOARDGRABV2_H
#define ZWPINPUTMETHODKEYBOARDGRABV2_H

#include "Type.h"

namespace wl {

class ZwpInputMethodKeyboardGrabV2 : public Type<zwp_input_method_keyboard_grab_v2>
{
public:
    ZwpInputMethodKeyboardGrabV2(zwp_input_method_keyboard_grab_v2 *val);
    ~ZwpInputMethodKeyboardGrabV2();
};

}; // namespace wl

#endif // !ZWPINPUTMETHODKEYBOARDGRABV2_H

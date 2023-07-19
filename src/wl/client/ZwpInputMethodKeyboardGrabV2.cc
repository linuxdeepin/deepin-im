#include "ZwpInputMethodKeyboardGrabV2.h"

using namespace wl::client;

ZwpInputMethodKeyboardGrabV2::ZwpInputMethodKeyboardGrabV2(zwp_input_method_keyboard_grab_v2 *val)
    : Type(val)
{
}

ZwpInputMethodKeyboardGrabV2::~ZwpInputMethodKeyboardGrabV2()
{
    zwp_input_method_keyboard_grab_v2_destroy(get());
}

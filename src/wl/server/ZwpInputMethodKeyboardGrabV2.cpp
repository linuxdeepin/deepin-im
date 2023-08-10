#include "ZwpInputMethodKeyboardGrabV2.h"

using namespace wl::server;

const struct zwp_input_method_keyboard_grab_v2_interface ZwpInputMethodKeyboardGrabV2::impl = {
    ResourceCallbackWrapper<&ZwpInputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_release>::func,
};

ZwpInputMethodKeyboardGrabV2::ZwpInputMethodKeyboardGrabV2()
    : Type()
{
}

ZwpInputMethodKeyboardGrabV2::~ZwpInputMethodKeyboardGrabV2() { }

#include "ZwpInputMethodManagerV2.h"

using namespace wl;

const struct zwp_input_method_manager_v2_interface ZwpInputMethodManagerV2::impl = {};

void ZwpInputMethodManagerV2::getInputMethod([[maybe_unused]] struct wl_client *client,
                                             [[maybe_unused]] struct wl_resource *resource,
                                             [[maybe_unused]] struct wl_resource *seat,
                                             [[maybe_unused]] uint32_t input_method)
{
}

void ZwpInputMethodManagerV2::destroy([[maybe_unused]] struct wl_client *client,
                                      [[maybe_unused]] struct wl_resource *resource)
{
}

#include "ZwpInputMethodManagerV2.h"

#include "utils.h"

using namespace wl::server;

template<>
const struct zwp_input_method_manager_v2_interface ZwpInputMethodManagerV2::Type::impl = {
    ResourceCallbackWrapper<&ZwpInputMethodManagerV2::getInputMethod>::func,
    ResourceCallbackWrapper<&ZwpInputMethodManagerV2::destroy>::func,
};

ZwpInputMethodManagerV2::ZwpInputMethodManagerV2()
    : Type()
{
}

void ZwpInputMethodManagerV2::getInputMethod([[maybe_unused]] struct wl_client *client,
                                             [[maybe_unused]] struct wl_resource *seat,
                                             [[maybe_unused]] uint32_t input_method)
{
}

void ZwpInputMethodManagerV2::destroy([[maybe_unused]] struct wl_client *client) { }

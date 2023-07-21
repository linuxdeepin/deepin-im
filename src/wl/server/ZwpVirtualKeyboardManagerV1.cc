#include "ZwpVirtualKeyboardManagerV1.h"

using namespace wl::server;

template<>
const struct zwp_virtual_keyboard_manager_v1_interface ZwpVirtualKeyboardManagerV1::Type::impl = {
    ResourceCallbackWrapper<&ZwpVirtualKeyboardManagerV1::createVirtualKeyboard>::func,
};

ZwpVirtualKeyboardManagerV1::ZwpVirtualKeyboardManagerV1(struct wl_client *client, uint32_t id)
    : Type(client, id)
{
}

ZwpVirtualKeyboardManagerV1::~ZwpVirtualKeyboardManagerV1() { }

void ZwpVirtualKeyboardManagerV1::createVirtualKeyboard([[maybe_unused]] struct wl_client *client,
                                                        [[maybe_unused]] struct wl_resource *seat,
                                                        [[maybe_unused]] uint32_t id)
{
}

void ZwpVirtualKeyboardManagerV1::resourceDestroy() { }

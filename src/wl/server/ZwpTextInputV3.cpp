#include "ZwpTextInputV3.h"

using namespace wl::server;

template<>
const struct zwp_text_input_v3_interface ZwpTextInputV3::Type::impl = {
    ResourceCallbackWrapper<&ZwpTextInputV3::destroy>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::enable>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::disable>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::setSurroundingText>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::setTextChangeCause>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::setContentType>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::setCursorRectangle>::func,
    ResourceCallbackWrapper<&ZwpTextInputV3::commit>::func,
};

ZwpTextInputV3::ZwpTextInputV3(struct wl_client *client, uint32_t id)
    : Type(client, id)
{
}

void ZwpTextInputV3::destroy([[maybe_unused]] struct wl_client *client) { }

void ZwpTextInputV3::enable([[maybe_unused]] struct wl_client *client) { }

void ZwpTextInputV3::disable([[maybe_unused]] struct wl_client *client) { }

void ZwpTextInputV3::setSurroundingText([[maybe_unused]] struct wl_client *client,
                                        [[maybe_unused]] const char *text,
                                        [[maybe_unused]] int32_t cursor,
                                        [[maybe_unused]] int32_t anchor)
{
}

void ZwpTextInputV3::setTextChangeCause([[maybe_unused]] struct wl_client *client,
                                        [[maybe_unused]] uint32_t cause)
{
}

void ZwpTextInputV3::setContentType([[maybe_unused]] struct wl_client *client,
                                    [[maybe_unused]] uint32_t hint,
                                    [[maybe_unused]] uint32_t purpose)
{
}

void ZwpTextInputV3::setCursorRectangle([[maybe_unused]] struct wl_client *client,
                                        [[maybe_unused]] int32_t x,
                                        [[maybe_unused]] int32_t y,
                                        [[maybe_unused]] int32_t width,
                                        [[maybe_unused]] int32_t height)
{
}

void ZwpTextInputV3::commit([[maybe_unused]] struct wl_client *client) { }

void ZwpTextInputV3::resourceDestroy() { }

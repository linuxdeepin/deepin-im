#include "WaylandInputContextV2.h"

#include "utils.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"

using namespace org::deepin::dim;

[[maybe_unused]] static const zwp_input_method_v2_listener imListener = {
    CallbackWrapper<&WaylandInputContextV2::activate>::func,
    CallbackWrapper<&WaylandInputContextV2::deactivate>::func,
    CallbackWrapper<&WaylandInputContextV2::surroundingText>::func,
    CallbackWrapper<&WaylandInputContextV2::textChangeCause>::func,
    CallbackWrapper<&WaylandInputContextV2::contentType>::func,
    CallbackWrapper<&WaylandInputContextV2::done>::func,
    CallbackWrapper<&WaylandInputContextV2::unavailable>::func,
};

WaylandInputContextV2::WaylandInputContextV2(Dim *dim, QObject *parent)
    : InputContext(dim, parent)
{
}

void WaylandInputContextV2::activate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    // icid_ = id;
}

void WaylandInputContextV2::deactivate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    // icid_ = nullptr;
}

void WaylandInputContextV2::surroundingText(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] const char *text,
    [[maybe_unused]] uint32_t cursor,
    [[maybe_unused]] uint32_t anchor)
{
}

void WaylandInputContextV2::textChangeCause(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t cause)
{
}

void WaylandInputContextV2::contentType(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t hint,
    [[maybe_unused]] uint32_t purpose)
{
}

void WaylandInputContextV2::done([[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
}

void WaylandInputContextV2::unavailable(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
}
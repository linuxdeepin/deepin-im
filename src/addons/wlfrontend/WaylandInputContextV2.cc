#include "WaylandInputContextV2.h"

#include "utils.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"

#include <QDebug>

using namespace org::deepin::dim;

const zwp_input_method_v2_listener WaylandInputContextV2::im_listener_ = {
    CallbackWrapper<&WaylandInputContextV2::activate>::func,
    CallbackWrapper<&WaylandInputContextV2::deactivate>::func,
    CallbackWrapper<&WaylandInputContextV2::surroundingText>::func,
    CallbackWrapper<&WaylandInputContextV2::textChangeCause>::func,
    CallbackWrapper<&WaylandInputContextV2::contentType>::func,
    CallbackWrapper<&WaylandInputContextV2::done>::func,
    CallbackWrapper<&WaylandInputContextV2::unavailable>::func,
};

WaylandInputContextV2::WaylandInputContextV2(
    const std::shared_ptr<WlType<zwp_input_method_v2>> &im,
    const std::shared_ptr<WlType<zwp_virtual_keyboard_v1>> &vk)
    : im_(im)
    , vk_(vk)
{
    zwp_input_method_v2_add_listener(im_->get(), &im_listener_, this);
}

void WaylandInputContextV2::activate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im activated:" << id();
}

void WaylandInputContextV2::deactivate(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im deactivated:" << id();
}

void WaylandInputContextV2::surroundingText(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] const char *text,
    [[maybe_unused]] uint32_t cursor,
    [[maybe_unused]] uint32_t anchor)
{
    qWarning() << "im surroundingText:" << id();
}

void WaylandInputContextV2::textChangeCause(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t cause)
{
    qWarning() << "im textChangeCause:" << id();
}

void WaylandInputContextV2::contentType(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2,
    [[maybe_unused]] uint32_t hint,
    [[maybe_unused]] uint32_t purpose)
{
    qWarning() << "im contentType:" << id();
}

void WaylandInputContextV2::done([[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im done:" << id();
}

void WaylandInputContextV2::unavailable(
    [[maybe_unused]] struct zwp_input_method_v2 *zwp_input_method_v2)
{
    qWarning() << "im unvailable:" << id();
}

// void WaylandInputContextV2::updatePreedit([[maybe_unused]] const org::deepin::dim::PreeditKey
// &key)
// {
// }

// void WaylandInputContextV2::updateCommitString([[maybe_unused]] const QString &text) { }

// void WaylandInputContextV2::forwardKey([[maybe_unused]] const org::deepin::dim::ForwardKey &key)
// { }
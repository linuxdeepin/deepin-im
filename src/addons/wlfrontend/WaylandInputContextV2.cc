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

const zwp_input_method_keyboard_grab_v2_listener WaylandInputContextV2::grab_listener_ = {
    CallbackWrapper<&WaylandInputContextV2::keymap>::func,
    CallbackWrapper<&WaylandInputContextV2::key>::func,
    CallbackWrapper<&WaylandInputContextV2::modifiers>::func,
    CallbackWrapper<&WaylandInputContextV2::repeatInfo>::func,
};

WaylandInputContextV2::WaylandInputContextV2(
    Dim *dim,
    const std::shared_ptr<WlType<zwp_input_method_v2>> &im,
    const std::shared_ptr<WlType<zwp_virtual_keyboard_v1>> &vk)
    : InputContext(dim)
    , im_(im)
    , vk_(vk)
    , state_(std::make_unique<State>())
{
    zwp_input_method_v2_add_listener(im_->get(), &im_listener_, this);

    grab_ = std::make_shared<WlType<zwp_input_method_keyboard_grab_v2>>(
        zwp_input_method_v2_grab_keyboard(im_->get()));
    zwp_input_method_keyboard_grab_v2_add_listener(grab_->get(), &grab_listener_, this);
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

void WaylandInputContextV2::keymap(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t format,
    int32_t fd,
    uint32_t size)
{
    qWarning() << "grab keymap:" << format << fd << size;
}

void WaylandInputContextV2::key(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t serial,
    uint32_t time,
    uint32_t key,
    uint32_t state)
{
    qWarning() << "grab key:" << serial << time << key << state;
    zwp_input_method_v2_commit_string(im_->get(), "test");
    zwp_input_method_v2_commit(im_->get(), state_->serial++);
}

void WaylandInputContextV2::modifiers(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    uint32_t serial,
    uint32_t mods_depressed,
    uint32_t mods_latched,
    uint32_t mods_locked,
    uint32_t group)
{
    qWarning() << "grab modifiers:" << serial << mods_depressed << mods_latched << mods_locked
               << group;
}

void WaylandInputContextV2::repeatInfo(
    [[maybe_unused]] struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
    int32_t rate,
    int32_t delay)
{
    qWarning() << "grab repeatInfo:" << rate << delay;
}

void WaylandInputContextV2::updatePreedit([[maybe_unused]] const org::deepin::dim::PreeditKey &key)
{
}

void WaylandInputContextV2::updateCommitString([[maybe_unused]] const QString &text) { }

void WaylandInputContextV2::forwardKey([[maybe_unused]] const org::deepin::dim::ForwardKey &key) { }

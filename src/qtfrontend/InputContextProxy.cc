#include "InputContextProxy.h"

#include "common/common.h"
#include "wl/client/Connection.h"
#include "wl/client/Seat.h"
#include "wl/client/ZwpTextInputManagerV3.h"
#include "wl/client/ZwpTextInputV3.h"

#include <wayland-text-input-unstable-v3-client-protocol.h>

#include <QDebug>
#include <QList>

const zwp_text_input_v3_listener InputContextProxy::tiListener = {
    CallbackWrapper<&InputContextProxy::enter>::func,
    CallbackWrapper<&InputContextProxy::leave>::func,
    CallbackWrapper<&InputContextProxy::preedit_string>::func,
    CallbackWrapper<&InputContextProxy::commit_string>::func,
    CallbackWrapper<&InputContextProxy::delete_surrounding_text>::func,
    CallbackWrapper<&InputContextProxy::done>::func,
};

InputContextProxy::InputContextProxy(QObject *parent)
    : QObject(parent)
    , available_(true)
{
    std::string waylandServer;
    const char *waylandServerCStr = getenv("WAYLAND_DISPLAY");
    if (waylandServerCStr == nullptr) {
        qWarning() << "WAYLAND_DISPLAY is empty";
        // TODO:
    }
    waylandServer = waylandServerCStr;

    wl_ = new wl::client::Connection(waylandServer, this);

    auto seats = wl_->getGlobals<wl::client::Seat>();
    auto tiManager = wl_->getGlobal<wl::client::ZwpTextInputManagerV3>();
    // TODO: select seat
    auto seat = seats[0];

    ti_ = tiManager->getTextInput(seat);

    // todo: select seat
    zwp_text_input_v3_add_listener(ti_->get(), &tiListener, this);
}

void InputContextProxy::focusIn()
{
    ti_->enable();
}

void InputContextProxy::focusOut()
{
    ti_->disable();
}

void InputContextProxy::processKeyEvent([[maybe_unused]] uint keyval,
                                        [[maybe_unused]] uint keycode,
                                        [[maybe_unused]] uint state,
                                        [[maybe_unused]] bool isRelease,
                                        [[maybe_unused]] uint time)
{
}

void InputContextProxy::enter([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                              [[maybe_unused]] struct wl_surface *surface)
{
}

void InputContextProxy::leave([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                              [[maybe_unused]] struct wl_surface *surface)
{
}

void InputContextProxy::preedit_string([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                                       const char *text,
                                       [[maybe_unused]] int32_t cursor_begin,
                                       [[maybe_unused]] int32_t cursor_end)
{
    QStringList data;
    data << text;
    // todo: split by cursor
    emit preedit(data);
}

void InputContextProxy::commit_string([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                                      const char *text)
{
    emit commitString(text);
}

void InputContextProxy::delete_surrounding_text(
    [[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
    [[maybe_unused]] uint32_t before_length,
    [[maybe_unused]] uint32_t after_length)
{
}

void InputContextProxy::done([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                             [[maybe_unused]] uint32_t serial)
{
}

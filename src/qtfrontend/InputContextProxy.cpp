#include "InputContextProxy.h"

#include "common/common.h"
#include "wayland-text-input-unstable-v3-client-protocol.h"
#include "wl/client/ConnectionRaw.h"
#include "wl/client/Seat.h"
#include "wl/client/ZwpTextInputManagerV3.h"
#include "wl/client/ZwpTextInputV3.h"

#include <qpa/qplatformnativeinterface.h>

#include <QDebug>
#include <QGuiApplication>
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
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    struct wl_display *wl_dpy =
        (struct wl_display *)native->nativeResourceForWindow("display", NULL);

    wl_ = new wl::client::ConnectionRaw(wl_dpy, this);

    auto seats = wl_->getGlobals<wl::client::Seat>();
    auto tiManager = wl_->getGlobal<wl::client::ZwpTextInputManagerV3>();
    // TODO: select seat
    auto seat = seats[0];

    ti_ = tiManager->getTextInput(seat);

    // todo: select seat
    zwp_text_input_v3_add_listener(ti_->get(), &tiListener, this);
    wl_->flush();
}

void InputContextProxy::focusIn()
{
    ti_->enable();
    ti_->setSurroundingText("", 0, 0);
    ti_->setTextChangeCause(ZWP_TEXT_INPUT_V3_CHANGE_CAUSE_INPUT_METHOD);
    ti_->setContentType(ZWP_TEXT_INPUT_V3_CONTENT_HINT_NONE,
                        ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NORMAL);
    ti_->setCursorRectangle(62, 76, 0, 32);
    ti_->commit();
    wl_->flush();
}

void InputContextProxy::focusOut()
{
    ti_->disable();
    ti_->commit();
    wl_->flush();
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
    qWarning() << "enter";
}

void InputContextProxy::leave([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                              [[maybe_unused]] struct wl_surface *surface)
{
    qWarning() << "leave";
}

void InputContextProxy::preedit_string([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                                       const char *text,
                                       [[maybe_unused]] int32_t cursor_begin,
                                       [[maybe_unused]] int32_t cursor_end)
{
    qWarning() << "preedit";
    QStringList data;
    data << text;
    // todo: split by cursor
    emit preedit(data);
}

void InputContextProxy::commit_string([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                                      const char *text)
{
    qWarning() << "commit";
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
    qWarning() << "done";
}

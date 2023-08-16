// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputContextProxy.h"

#include "common/common.h"
#include "wayland-dim-text-input-unstable-v1-client-protocol.h"
#include "wl/client/Connection.h"
#include "wl/client/ConnectionRaw.h"
#include "wl/client/Seat.h"
#include "wl/client/ZwpDimTextInputManagerV1.h"
#include "wl/client/ZwpDimTextInputV1.h"

#include <qpa/qplatformnativeinterface.h>

#include <QDebug>
#include <QGuiApplication>
#include <QList>

const zwp_dim_text_input_v1_listener InputContextProxy::tiListener = {
    CallbackWrapper<&InputContextProxy::modifiers_map>::func,
    CallbackWrapper<&InputContextProxy::preedit_string>::func,
    CallbackWrapper<&InputContextProxy::commit_string>::func,
    CallbackWrapper<&InputContextProxy::delete_surrounding_text>::func,
    CallbackWrapper<&InputContextProxy::done>::func,
    CallbackWrapper<&InputContextProxy::keysym>::func,
};

InputContextProxy::InputContextProxy(QObject *parent)
    : QObject(parent)
    , available_(true)
{
    if (QGuiApplication::platformName() == "wayland") {
        QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
        struct wl_display *wl_dpy =
            (struct wl_display *)native->nativeResourceForWindow("display", NULL);

        wl_ = new wl::client::ConnectionRaw(wl_dpy, this);
    } else {
        wl_ = new wl::client::Connection("imfakewl", this);
    }

    auto seat = wl_->getGlobal<wl::client::Seat>();
    auto tiManager = wl_->getGlobal<wl::client::ZwpDimTextInputManagerV1>();

    ti_ = tiManager->getTextInput(seat);

    zwp_dim_text_input_v1_add_listener(ti_->get(), &tiListener, this);
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

void InputContextProxy::modifiers_map(struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                      struct wl_array *map)
{
    qWarning() << "modifiers_map";
}

void InputContextProxy::preedit_string(
    [[maybe_unused]] struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
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

void InputContextProxy::commit_string(
    [[maybe_unused]] struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1, const char *text)
{
    qWarning() << "commit";
    emit commitString(text);
}

void InputContextProxy::delete_surrounding_text(
    [[maybe_unused]] struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
    [[maybe_unused]] uint32_t before_length,
    [[maybe_unused]] uint32_t after_length)
{
}

void InputContextProxy::done([[maybe_unused]] struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                             [[maybe_unused]] uint32_t serial)
{
    qWarning() << "done";
}

void InputContextProxy::keysym(struct zwp_dim_text_input_v1 *zwp_dim_text_input_v1,
                                uint32_t serial,
                               uint32_t time,
                               uint32_t sym,
                               uint32_t state,
                               uint32_t modifiers)
{
    qWarning() << "keysym";
}

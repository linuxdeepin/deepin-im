// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodKeyboardGrabV2.h"

#include "X11KeyboardGrabber.h"

#include <QDateTime>
#include <QDebug>

InputMethodKeyboardGrabV2::InputMethodKeyboardGrabV2(struct ::wl_resource *seat)
    : seat_(seat)
    , grabber_(std::make_unique<X11KeyboardGrabber>())
{
    QObject::connect(grabber_.get(),
                     &X11KeyboardGrabber::keyEvent,
                     [this](int keycode, bool isRelease) {
                         sendKey(keycode, isRelease);
                     });
}

InputMethodKeyboardGrabV2::~InputMethodKeyboardGrabV2() { }

void InputMethodKeyboardGrabV2::sendKey(uint32_t key, uint32_t state)
{
    uint32_t ts = QDateTime::currentSecsSinceEpoch();
    const auto resources = resourceMap();
    for (auto r : resources) {
        // send_key(r->handle, nextSerial(), ts, key, state);
    }
}

void InputMethodKeyboardGrabV2::zwp_input_method_keyboard_grab_v2_release(
    wl::server::Resource *resource)
{
    resource->destroy();
}

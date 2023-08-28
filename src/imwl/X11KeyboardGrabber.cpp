// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "X11KeyboardGrabber.h"

#include <xcb/xinput.h>

#include <QDebug>

#define XCB_REPLY_CONNECTION_ARG(connection, ...) connection
#define XCB_REPLY(call, ...)       \
  std::unique_ptr<call##_reply_t>( \
      call##_reply(XCB_REPLY_CONNECTION_ARG(__VA_ARGS__), call(__VA_ARGS__), nullptr))

X11KeyboardGrabber::X11KeyboardGrabber()
    : XCB()
{
    setup_ = xcb_get_setup(xconn_.get());
    screen_ = screenOfDisplay(defaultScreenNbr_);

    initXinputExtension();
}

X11KeyboardGrabber::~X11KeyboardGrabber() { }

void X11KeyboardGrabber::xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event)
{
    auto responseType = event->response_type & ~0x80;
    if (responseType != XCB_GE_GENERIC) {
        return;
    }

    auto *ge = reinterpret_cast<xcb_ge_generic_event_t *>(event.get());
    if (ge->extension != xinput2OPCode_) {
        return;
    }

    if (ge->event_type != XCB_INPUT_RAW_KEY_PRESS && ge->event_type != XCB_INPUT_RAW_KEY_RELEASE) {
        return;
    }

    bool isRelease = ge->event_type == XCB_INPUT_RAW_KEY_RELEASE;
    auto *ke = reinterpret_cast<xcb_input_raw_key_press_event_t *>(event.get());

    emit keyEvent(ke->detail, isRelease);
}

xcb_screen_t *X11KeyboardGrabber::screenOfDisplay(int screen)
{
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup_);
    for (; iter.rem; --screen, xcb_screen_next(&iter)) {
        if (screen == 0) {
            return iter.data;
        }
    }

    return nullptr;
}

void X11KeyboardGrabber::initXinputExtension()
{
    {
        const char *extname = "XInputExtension";
        auto reply = XCB_REPLY(xcb_query_extension, xconn_.get(), strlen(extname), extname);
        if (!reply->present) {
            throw std::runtime_error("XInput extension is not available");
        }
        xinput2OPCode_ = reply->major_opcode;
    }

    {
        auto reply = XCB_REPLY(xcb_input_xi_query_version,
                               xconn_.get(),
                               XCB_INPUT_MAJOR_VERSION,
                               XCB_INPUT_MINOR_VERSION);
        if (!reply || reply->major_version != 2) {
            throw std::runtime_error("X server does not support XInput 2");
        }

        qDebug() << "XInput version:" << reply->major_version << "." << reply->minor_version;
    }

    struct
    {
        xcb_input_event_mask_t head;
        xcb_input_xi_event_mask_t mask;
    } mask;

    mask.head.deviceid = XCB_INPUT_DEVICE_ALL_MASTER;
    mask.head.mask_len = sizeof(mask.mask) / sizeof(uint32_t);
    mask.mask = static_cast<xcb_input_xi_event_mask_t>(XCB_INPUT_XI_EVENT_MASK_RAW_KEY_PRESS
                                                       | XCB_INPUT_XI_EVENT_MASK_RAW_KEY_RELEASE);
    auto cookie = xcb_input_xi_select_events(xconn_.get(), screen_->root, 1, &mask.head);
    auto err = xcb_request_check(xconn_.get(), cookie);
    if (err) {
        throw std::runtime_error("xcb_input_xi_select_events failed");
    }
}

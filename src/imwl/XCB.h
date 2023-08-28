// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef XCB_H
#define XCB_H

#include "common/common.h"

#include <xcb/xcb.h>

#include <QObject>
#include <QSocketNotifier>

#define XCB_REPLY_CONNECTION_ARG(connection, ...) connection
#define XCB_REPLY(call, ...)       \
  std::unique_ptr<call##_reply_t>( \
      call##_reply(XCB_REPLY_CONNECTION_ARG(__VA_ARGS__), call(__VA_ARGS__), nullptr))

/**
 * @brief Bit mask to find event type regardless of event source.
 *
 * Each event in the X11 protocol contains an 8-bit type code.
 * The most-significant bit in this code is set if the event was
 * generated from a SendEvent request. This mask can be used to
 * determine the type of event regardless of how the event was
 * generated. See the X11R6 protocol specification for details.
 */
#define XCB_EVENT_RESPONSE_TYPE_MASK (0x7f)
#define XCB_EVENT_RESPONSE_TYPE(e)   (e->response_type &  XCB_EVENT_RESPONSE_TYPE_MASK)
#define XCB_EVENT_SENT(e)            (e->response_type & ~XCB_EVENT_RESPONSE_TYPE_MASK)

class XCB : public QObject
{
    Q_OBJECT

public:
    XCB();
    virtual ~XCB();

protected:
    std::unique_ptr<xcb_connection_t, Deleter<xcb_disconnect>> xconn_;
    int defaultScreenNbr_;

    xcb_screen_t *screen() { return screen_; }

    virtual void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) = 0;

private:
    int xcbFd_;
    QSocketNotifier *socketNotifier_;
    const xcb_setup_t *setup_;
    xcb_screen_t *screen_;

    xcb_screen_t *screenOfDisplay(int screen);
    void onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent);
};

#endif // !XCB_H

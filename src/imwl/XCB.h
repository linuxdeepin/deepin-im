// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef XCB_H
#define XCB_H

#include "common/common.h"

#include <xcb/xcb.h>

#include <QObject>
#include <QSocketNotifier>

class XCB: public QObject
{
    Q_OBJECT

public:
    XCB();
    virtual ~XCB();

protected:
    std::unique_ptr<xcb_connection_t, Deleter<xcb_disconnect>> xconn_;
    int defaultScreenNbr_;

    virtual void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) = 0;

private:
    int xcbFd_;
    QSocketNotifier *socketNotifier_;

    xcb_screen_t *screenOfDisplay(int screen);
    void onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent);
};

#endif // !XCB_H

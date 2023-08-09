// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef X11KEYBOARDGRABBER_H
#define X11KEYBOARDGRABBER_H

#include "common/common.h"

#include <xcb/xcb.h>

#include <QObject>
#include <QSocketNotifier>

class X11KeyboardGrabber : public QObject
{
    Q_OBJECT
public:
    X11KeyboardGrabber();
    ~X11KeyboardGrabber();

signals:
    void keyEvent(int keycode, bool isRelease);

private:
    std::unique_ptr<xcb_connection_t, Deleter<xcb_disconnect>> xconn_;
    int xcbFd_;
    QSocketNotifier *socketNotifier_;
    const xcb_setup_t *setup_;
    xcb_screen_t *screen_;
    uint8_t xinput2OPCode_;

    xcb_screen_t *screenOfDisplay(int screen);
    void onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent);
    void initXinputExtension();
};

#endif // !X11KEYBOARDGRABBER_H

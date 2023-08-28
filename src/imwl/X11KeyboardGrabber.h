// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef X11KEYBOARDGRABBER_H
#define X11KEYBOARDGRABBER_H

#include "XCB.h"

#include <xcb/xcb.h>

#include <QObject>
#include <QSocketNotifier>

class X11KeyboardGrabber : public XCB
{
    Q_OBJECT

public:
    X11KeyboardGrabber();
    virtual ~X11KeyboardGrabber();

signals:
    void keyEvent(int keycode, bool isRelease);

protected:
    void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) override;

private:
    int xcbFd_;
    uint8_t xinput2OPCode_;

    void onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent);
    void initXinputExtension();
};

#endif // !X11KEYBOARDGRABBER_H

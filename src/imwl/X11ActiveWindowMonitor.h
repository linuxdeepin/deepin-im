// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef X11ACTIVEWINDOWMONITOR_H
#define X11ACTIVEWINDOWMONITOR_H

#include "Xcb.h"

class X11ActiveWindowMonitor : public Xcb
{
    Q_OBJECT

public:
    X11ActiveWindowMonitor();
    ~X11ActiveWindowMonitor();

    pid_t activeWindowPid();
    std::tuple<uint16_t, uint16_t> activeWindowPosition();

signals:
    void activeWindowChanged();

protected:
    void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) override;

private:
    const std::string activeWindow_;
    const std::string wmPid_;
};

#endif // !X11ACTIVEWINDOWMONITOR_H

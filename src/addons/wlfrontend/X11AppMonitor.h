// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef XCBAPPMONITOR_H
#define XCBAPPMONITOR_H

#include "AppMonitor.h"
#include "Xcb.h"

#include <memory>

namespace org::deepin::dim {

class X11AppMonitor : public AppMonitor, public Xcb
{
public:
    X11AppMonitor();
    ~X11AppMonitor() override;

    const std::unordered_map<QString, pid_t> &apps() { return apps_; }

protected:
    void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) override;

private:
    const std::string activeWindow_;
    const std::string netClientList_;
    const std::string wmPid_;

    std::unordered_map<QString, pid_t> apps_;
    QString focus_;

    void init();
    pid_t getWindowPid(xcb_window_t window);
    std::tuple<uint16_t, uint16_t> getWindowPosition(xcb_window_t window);
    void activeWindowChanged();
    void clientListChanged();
};

} // namespace org::deepin::dim

#endif // !XCBAPPMONITOR_H

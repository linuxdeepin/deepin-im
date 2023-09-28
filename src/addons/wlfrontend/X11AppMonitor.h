#ifndef XCBAPPMONITOR_H
#define XCBAPPMONITOR_H

#include "AppMonitor.h"
#include "Xcb.h"

#include <memory>

class X11ActiveWindowMonitor;

class X11AppMonitor : public AppMonitor, public Xcb
{
public:
    X11AppMonitor();
    ~X11AppMonitor() override;

protected:
    void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) override;

private:
    xcb_atom_t atomActiveWindow_;
    xcb_atom_t atomNetClientList_;
    xcb_atom_t atomWmPid_;

    std::unordered_map<QString, pid_t> apps_;
    QString focus_;

    void init();
    pid_t getWindowPid(xcb_window_t window);
    std::tuple<uint16_t, uint16_t> getWindowPosition(xcb_window_t window);
    
    QString windowToString(xcb_window_t window);
    xcb_window_t stringToWindow(const QString &string);
};

#endif // !XCBAPPMONITOR_H

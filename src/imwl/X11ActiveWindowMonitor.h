#ifndef X11ACTIVEWINDOWMONITOR_H
#define X11ACTIVEWINDOWMONITOR_H

#include "XCB.h"

class X11ActiveWindowMonitor : public XCB
{
    Q_OBJECT

public:
    X11ActiveWindowMonitor();
    ~X11ActiveWindowMonitor();

signals:
    void activeWindowChanged(uint32_t pid);

protected:
    void xcbEvent(const std::unique_ptr<xcb_generic_event_t> &event) override;

private:
    xcb_atom_t activeWindowAtom_;
    xcb_atom_t wmPidAtom_;
};

#endif // !X11ACTIVEWINDOWMONITOR_H

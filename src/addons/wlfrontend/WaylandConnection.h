#ifndef WAYLANDCONNECTION_H
#define WAYLANDCONNECTION_H

#include "common/common.h"

#include <wayland-client-core.h>

#include <QSocketNotifier>

#include <memory>
#include <string>

struct wl_display;

class WaylandConnection : public QObject
{
public:
    WaylandConnection(const std::string &name, QObject *parent = nullptr);
    ~WaylandConnection();

    wl_display *display() const { return display_.get(); }

    void roundtrip();
    void flush();

private:
    std::unique_ptr<wl_display, Deleter<&wl_display_disconnect>> display_;
    int fd_;
    QSocketNotifier *notifier_;

    void init();
    void dispatch();
};

#endif // !WAYLANDCONNECTION_H

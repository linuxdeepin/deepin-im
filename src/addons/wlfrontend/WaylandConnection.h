#ifndef WAYLANDCONNECTION_H
#define WAYLANDCONNECTION_H

#include <QSocketNotifier>

#include <string>

struct wl_display;

class WaylandConnection : public QObject
{
public:
    WaylandConnection(const std::string &name, QObject *parent = nullptr);
    ~WaylandConnection();

    wl_display *display() const { return display_; }

    void roundtrip();
    void flush();

private:
    wl_display *display_;
    int fd_;
    QSocketNotifier *notifier_;

    void init();
    void dispatch();
};

#endif // !WAYLANDCONNECTION_H

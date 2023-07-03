#ifndef WAYLANDCONNECTION_H
#define WAYLANDCONNECTION_H

#include <QSocketNotifier>

#include <string>

struct wl_display;

namespace org {
namespace deepin {
namespace dim {

class WaylandConnection : public QObject
{
public:
    WaylandConnection(const std::string &name, QObject *parent = nullptr);
    ~WaylandConnection();

    wl_display *display() const { return display_; }

    void roundTrip();

private:
    wl_display *display_;
    int fd_;
    QSocketNotifier *notifier_;

    void init();
    void dispatch();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !WAYLANDCONNECTION_H

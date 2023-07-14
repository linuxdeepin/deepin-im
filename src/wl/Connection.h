#ifndef WL_CONNECTION_H
#define WL_CONNECTION_H

#include "common/common.h"

#include <wayland-client-core.h>

#include <QSocketNotifier>

#include <memory>
#include <string>

struct wl_display;

namespace wl {

class Connection : public QObject
{
public:
    Connection(const std::string &name, QObject *parent = nullptr);
    ~Connection();

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

} // namespace wl

#endif // !WL_CONNECTION_H

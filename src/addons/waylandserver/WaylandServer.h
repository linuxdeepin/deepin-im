#ifndef WAYLANDSERVER_H
#define WAYLANDSERVER_H

#include "dimcore/Addon.h"

#include <QObject>

struct wl_display;
struct wlr_backend;

namespace wl::client {
class ConnectionBase;
}

namespace org::deepin::dim {

class WaylandServer : public Addon
{
    Q_OBJECT

public:
    explicit WaylandServer(Dim *dim);
    ~WaylandServer();

    std::shared_ptr<wl_display> getRemote();
    std::shared_ptr<wl_display> getLocal();
    std::shared_ptr<wlr_backend> getBackend();

private:
    std::shared_ptr<wl_display> remote_;
    std::shared_ptr<wl_display> display_;
    std::shared_ptr<wlr_backend> backend_;
};

} // namespace org::deepin::dim

#endif // !WAYLANDSERVER_H

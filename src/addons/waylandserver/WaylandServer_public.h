#ifndef WAYLANDSERVER_PUBLIC_H
#define WAYLANDSERVER_PUBLIC_H

#include <memory>

struct wl_display;
struct wlr_backend;

namespace wl::client {
class ConnectionBase;
}

namespace org::deepin::dim {

class Addon;

namespace waylandserver {

std::shared_ptr<wl_display> getRemote(Addon *addon);
std::shared_ptr<wl_display> getLocal(Addon *addon);
std::shared_ptr<wlr_backend> getBackend(Addon *addon);

} // namespace waylandserver

} // namespace org::deepin::dim

#endif // !WAYLANDSERVER_PUBLIC_H

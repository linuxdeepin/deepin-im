#ifndef WL_SERVER_H
#define WL_SERVER_H

#include "common/common.h"

#include <wayland-server-core.h>

#include <memory>
#include <string>

namespace wl {
namespace server {

class Server
{
public:
    Server();
    ~Server() = default;

    wl_display *display() const { return display_.get(); }

    bool addSocket(const std::string &name);
    void run();

private:
    std::unique_ptr<wl_display, Deleter<wl_display_destroy>> display_;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_H

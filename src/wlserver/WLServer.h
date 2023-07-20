#ifndef WLSERVER_WLSERVER_H
#define WLSERVER_WLSERVER_H

#include <memory>

namespace wl {
namespace server {
class Server;
class Global;
class Seat;
class ZwpTextInputManagerV3;
class ZwpInputMethodManagerV2;
} // namespace server
} // namespace wl

class WLServer
{
public:
    WLServer();

    void run();

private:
    std::shared_ptr<wl::server::Server> server_;
    std::shared_ptr<wl::server::Global> seatGlobal_;
    std::shared_ptr<wl::server::Seat> seat_;
    std::shared_ptr<wl::server::Global> textInputManagerGlobal_;
    std::shared_ptr<wl::server::ZwpTextInputManagerV3> textInputManager_;
    std::shared_ptr<wl::server::Global> inputMethodManagerGlobal_;
    std::shared_ptr<wl::server::ZwpInputMethodManagerV2> inputMethodManager_;
};

#endif // !WLSERVER_WLSERVER_H

#ifndef WLSERVER_WLSERVER_H
#define WLSERVER_WLSERVER_H

#include "common/common.h"

#include <libudev.h>
#include <libinput.h>

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
    std::unique_ptr<struct udev, Deleter<udev_unref>> udev_;
    std::unique_ptr<struct libinput, Deleter<libinput_unref>> li_;

    std::shared_ptr<wl::server::Server> server_;
    std::shared_ptr<wl::server::Global> seatGlobal_;
    std::shared_ptr<wl::server::Seat> seat_;
    std::shared_ptr<wl::server::Global> textInputManagerGlobal_;
    std::shared_ptr<wl::server::ZwpTextInputManagerV3> textInputManager_;
    std::shared_ptr<wl::server::Global> inputMethodManagerGlobal_;
    std::shared_ptr<wl::server::ZwpInputMethodManagerV2> inputMethodManager_;

    void processLibinputEvents();
    void deviceAdded(struct libinput_event *event);
    void deviceRemoved(struct libinput_event *event);
};

#endif // !WLSERVER_WLSERVER_H

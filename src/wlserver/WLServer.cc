#include "WLServer.h"

#include "wl/server/Global.h"
#include "wl/server/Seat.h"
#include "wl/server/Server.h"
#include "wl/server/ZwpInputMethodManagerV2.h"
#include "wl/server/ZwpTextInputManagerV3.h"

#include <iostream>

#include <fcntl.h>
#include <unistd.h>

static int open_restricted(const char *path, int flags, [[maybe_unused]] void *user_data)
{
    std::cerr << "xxxxxxxxxxxxx" << std::endl;
    std::cerr << std::flush;
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, [[maybe_unused]] void *user_data)
{
    std::cerr << "xxxxxxxxxxxxx" << std::endl;
    std::cerr << std::flush;
    close(fd);
}

static const struct libinput_interface interface = {
    open_restricted,
    close_restricted,
};

WLServer::WLServer()
    : udev_(udev_new())
    , li_(libinput_udev_create_context(&interface, this, udev_.get()))
    , server_(std::make_shared<wl::server::Server>())
{
    server_->addSocket("imfakewl");

    seatGlobal_ = std::make_shared<wl::server::Global>(
        server_,
        wl::server::Seat::wl_interface,
        [this](struct wl_client *client, [[maybe_unused]] uint32_t version, uint32_t id) {
            seat_ = std::make_shared<wl::server::Seat>(client, id);
        });
    textInputManagerGlobal_ = std::make_shared<wl::server::Global>(
        server_,
        wl::server::ZwpTextInputManagerV3::wl_interface,
        [this](struct wl_client *client, [[maybe_unused]] uint32_t version, uint32_t id) {
            textInputManager_ = std::make_shared<wl::server::ZwpTextInputManagerV3>(client, id);
        });
    inputMethodManagerGlobal_ = std::make_shared<wl::server::Global>(
        server_,
        wl::server::ZwpInputMethodManagerV2::wl_interface,
        [this](struct wl_client *client, [[maybe_unused]] uint32_t version, uint32_t id) {
            inputMethodManager_ = std::make_shared<wl::server::ZwpInputMethodManagerV2>(client, id);
        });

    std::cerr << "xxxxxxxxxxxxx" << std::endl;
    std::cerr << std::flush;

    auto *loop = wl_display_get_event_loop(server_->display());
    int lifd = libinput_get_fd(li_.get());
    wl_event_loop_add_fd(
        loop,
        lifd,
        WL_EVENT_READABLE,
        []([[maybe_unused]] int fd, [[maybe_unused]] uint32_t mask, void *data) -> int {
            std::cerr << "xxxxxxxxxxxxx" << std::endl;
            std::cerr << std::flush;
            auto *wlserver = static_cast<WLServer *>(data);
            int res = libinput_dispatch(wlserver->li_.get());
            wlserver->processLibinputEvents();
            return res != 0;
        },
        this);
    std::cerr << "xxxxxxxxxxxxx" << std::endl;
    std::cerr << std::flush;

    wl_event_loop_add_timer(
        loop,
        []([[maybe_unused]] void *data) -> int {
            std::cerr << "uuuuuuuuuuuuuuuu" << std::endl;
            std::cerr << std::flush;
            return 0;
        },
        this);
}

void WLServer::run()
{
    server_->run();
}

void WLServer::processLibinputEvents()
{
    struct libinput_event *event;

    while ((event = libinput_get_event(li_.get())) != NULL) {
        enum libinput_event_type type = libinput_event_get_type(event);

        switch (type) {
        case LIBINPUT_EVENT_DEVICE_ADDED:
            deviceAdded(event);
            break;
        case LIBINPUT_EVENT_DEVICE_REMOVED:
            deviceRemoved(event);
            break;
        case LIBINPUT_EVENT_TOUCH_DOWN:
        case LIBINPUT_EVENT_TOUCH_UP:
        case LIBINPUT_EVENT_TOUCH_MOTION:
        case LIBINPUT_EVENT_TOUCH_CANCEL:
        case LIBINPUT_EVENT_TOUCH_FRAME:

        default:
            break;
        }

        libinput_event_destroy(event);
    }
}

void WLServer::deviceAdded(struct libinput_event *event)
{
    auto *dev = libinput_event_get_device(event);
    auto *seat = libinput_device_get_seat(dev);
    std::string seatName = libinput_seat_get_logical_name(seat);
    std::cerr << seatName << std::endl;
    std::cerr << std::flush;
}

void WLServer::deviceRemoved([[maybe_unused]] struct libinput_event *event) { }

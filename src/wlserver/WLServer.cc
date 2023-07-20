#include "WLServer.h"

#include "wl/server/Global.h"
#include "wl/server/Seat.h"
#include "wl/server/Server.h"
#include "wl/server/ZwpInputMethodManagerV2.h"
#include "wl/server/ZwpTextInputManagerV3.h"

WLServer::WLServer()
    : server_(std::make_shared<wl::server::Server>())
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
}

void WLServer::run()
{
    server_->run();
}

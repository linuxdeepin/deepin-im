#include "ConnectionFd.h"

using namespace wl::client;

ConnectionFd::ConnectionFd(struct wl_display *display, QObject *parent)
    : ConnectionBase(parent)
    , display_(display)
{
    init();
}

ConnectionFd::~ConnectionFd() { }

#include "ConnectionRaw.h"

using namespace wl::client;

ConnectionRaw::ConnectionRaw(struct wl_display *display, QObject *parent)
    : ConnectionBase(parent)
    , display_(display)
{
    init();
}

ConnectionRaw::~ConnectionRaw() { }

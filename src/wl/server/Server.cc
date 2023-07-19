#include "Server.h"

using namespace wl::server;

Server::Server()
    : display_(wl_display_create())
{
}

bool Server::addSocket(const std::string &name)
{
    return wl_display_add_socket(display_.get(), name.c_str()) == 0;
}

void Server::run()
{
    wl_display_run(display_.get());
}

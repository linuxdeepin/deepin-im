// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

std::unique_ptr<wl_event_source, Deleter<wl_event_source_remove>>
Server::eventLoopAddFd(int fd, uint32_t mask, wl_event_loop_fd_func_t func, void *data)
{
    std::unique_ptr<wl_event_source, Deleter<wl_event_source_remove>> ret;
    ret.reset(
        wl_event_loop_add_fd(wl_display_get_event_loop(display_.get()), fd, mask, func, data));

    return ret;
}

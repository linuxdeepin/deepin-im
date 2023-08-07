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

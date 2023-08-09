// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

    std::unique_ptr<wl_event_source, Deleter<wl_event_source_remove>>
    eventLoopAddFd(int fd, uint32_t mask, wl_event_loop_fd_func_t func, void *data);

private:
    std::unique_ptr<wl_display, Deleter<wl_display_destroy>> display_;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_H

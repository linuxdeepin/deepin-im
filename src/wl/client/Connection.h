// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_CONNECTION_H
#define WL_CLIENT_CONNECTION_H

#include "ConnectionBase.h"
#include "common/common.h"

#include <wayland-client-protocol.h>

#include <QSocketNotifier>

#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace wl {
namespace client {

class Connection : public ConnectionBase
{
public:
    Connection(const std::string &name, QObject *parent = nullptr);
    virtual ~Connection();

    struct wl_display *display() const override { return display_.get(); }

private:
    std::unique_ptr<wl_display, Deleter<&wl_display_disconnect>> display_;
    int fd_;
    QSocketNotifier *notifier_;

    void init();
    void dispatch();
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_CONNECTION_H

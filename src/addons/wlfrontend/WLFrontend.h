// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

#include <memory>

namespace wl {
namespace client {
class ConnectionBase;
class Seat;
} // namespace client
} // namespace wl

namespace org {
namespace deepin {
namespace dim {

class InputMethodV2;

class WLFrontend : public FrontendAddon
{
    Q_OBJECT

public:
    explicit WLFrontend(Dim *dim);
    ~WLFrontend();

private:
    std::unique_ptr<wl::client::ConnectionBase> wl_;

    std::unordered_map<std::shared_ptr<wl::client::Seat>, std::shared_ptr<InputMethodV2>> ims_;

    void init();
    void reloadSeats();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

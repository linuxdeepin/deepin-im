// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALKEYBOARDV1_H
#define VIRTUALKEYBOARDV1_H

#include "common.h"

#include <memory>

struct wl_client;
struct wl_display;
struct wl_resource;

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class VirtualKeyboardV1Private;

class VirtualKeyboardV1
{
    friend class VirtualKeyboardV1Private;

public:
    VirtualKeyboardV1(struct ::wl_resource *seat);
    ~VirtualKeyboardV1();

    INIT_FUNCS_DEF

private:
    std::unique_ptr<VirtualKeyboardV1Private> d;
    wl::server::Seat *seat_;
};

#endif // !VIRTUALKEYBOARDV1_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_TYPE_H
#define WL_CLIENT_TYPE_H

#include "wl/Type.h"

#include <wayland-client-protocol.h>

#include <string>

#include <stdint.h>

namespace wl {
namespace client {

template<typename T>
class Type : public wl::Type<T>
{
public:
    Type(T *val)
        : val_(val)
    {
    }

    ~Type() = default;

    T *get() { return val_; }

private:
    T *val_;
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_TYPE_H

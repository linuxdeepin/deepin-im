// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_ZWPTEXTINPUTMANAGERV3_H
#define WL_CLIENT_ZWPTEXTINPUTMANAGERV3_H

#include "Type.h"
#include "wayland-dim-text-input-unstable-v1-client-protocol.h"

#include <memory>

namespace wl {
namespace client {

class ZwpDimTextInputV1;
class Seat;

class ZwpDimTextInputManagerV1 : public Type<zwp_dim_text_input_manager_v1>
{
public:
    ZwpDimTextInputManagerV1(zwp_dim_text_input_manager_v1 *val);
    ~ZwpDimTextInputManagerV1();

    std::shared_ptr<ZwpDimTextInputV1> getTextInput(const std::shared_ptr<Seat> &seat);

private:
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_ZWPTEXTINPUTMANAGERV3_H

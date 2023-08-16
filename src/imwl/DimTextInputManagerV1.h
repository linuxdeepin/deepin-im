// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIMTEXTINPUTMANAGERV3_H
#define DIMTEXTINPUTMANAGERV3_H

#include "wl/server/ZwpDimTextInputManagerV1.h"

class Core;
class DimTextInputV1;

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class DimTextInputManagerV1 : public wl::server::ZwpDimTextInputManagerV1
{
public:
    DimTextInputManagerV1();
    ~DimTextInputManagerV1();

protected:
    void zwp_dim_text_input_manager_v1_destroy(wl::server::Resource *resource);
    void zwp_dim_text_input_manager_v1_get_text_input(wl::server::Resource *resource,
                                                  uint32_t id,
                                                  struct ::wl_resource *seat);
};

#endif // !DIMTEXTINPUTMANAGERV3_H

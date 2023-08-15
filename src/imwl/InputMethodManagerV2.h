// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODMANAGERV2_H
#define INPUTMETHODMANAGERV2_H

#include "wl/server/ZwpInputMethodManagerV2.h"

#include <memory>
#include <unordered_map>

class InputMethodV2;

class InputMethodManagerV2 : public wl::server::ZwpInputMethodManagerV2
{
public:
    InputMethodManagerV2();
    ~InputMethodManagerV2();

protected:
    void zwp_input_method_manager_v2_get_input_method(wl::server::Resource *resource,
                                                      struct ::wl_resource *seat,
                                                      uint32_t input_method) override;
    void zwp_input_method_manager_v2_destroy(wl::server::Resource *resource) override;
};

#endif // !INPUTMETHODMANAGERV2_H

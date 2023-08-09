// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODV2_H
#define INPUTMETHODV2_H

#include "common.h"

#include <memory>

struct wl_client;
struct wl_display;
struct wl_resource;

class Core;
class InputMethodV2Private;
class InputMethodKeyboardGrabV2;

class InputMethodV2
{
    friend class InputMethodV2Private;

public:
    InputMethodV2(Core *core, struct ::wl_resource *seat);
    ~InputMethodV2();

    INIT_FUNCS_DEF

    void sendDeactivate();
    void sendActivate();

private:
    std::unique_ptr<InputMethodV2Private> d;
    Core *core_;
    struct ::wl_resource *seat_;

    std::unique_ptr<InputMethodKeyboardGrabV2> grab_;
};

#endif // !INPUTMETHODV2_H

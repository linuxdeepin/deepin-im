// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODKEYBOARDGRABV2_H
#define INPUTMETHODKEYBOARDGRABV2_H

#include "common.h"

#include <memory>

struct wl_client;
struct wl_display;
struct wl_resource;

class InputMethodKeyboardGrabV2Private;
class X11KeyboardGrabber;

class InputMethodKeyboardGrabV2
{
    friend class InputMethodKeyboardGrabV2Private;

public:
    InputMethodKeyboardGrabV2(struct ::wl_resource *seat);
    ~InputMethodKeyboardGrabV2();

    INIT_FUNCS_DEF

private:
    std::unique_ptr<InputMethodKeyboardGrabV2Private> d;
    struct ::wl_resource *seat_;
    X11KeyboardGrabber *grabber_;
};

#endif // !INPUTMETHODKEYBOARDGRABV2_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODKEYBOARDGRABV2_H
#define INPUTMETHODKEYBOARDGRABV2_H

#include "wl/server/ZwpInputMethodKeyboardGrabV2.h"

#include <memory>

class X11KeyboardGrabber;

class InputMethodKeyboardGrabV2 : public wl::server::ZwpInputMethodKeyboardGrabV2
{
public:
    InputMethodKeyboardGrabV2(struct ::wl_resource *seat);
    ~InputMethodKeyboardGrabV2();

    void sendKey(uint32_t key, uint32_t state);

protected:
    virtual void zwp_input_method_keyboard_grab_v2_release(wl::server::Resource *resource) override;

private:
    struct ::wl_resource *seat_;
    std::unique_ptr<X11KeyboardGrabber> grabber_;
};

#endif // !INPUTMETHODKEYBOARDGRABV2_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODKEYBOARDGRABV2_H
#define INPUTMETHODKEYBOARDGRABV2_H

#include "wl/server/ZwpInputMethodKeyboardGrabV2.h"

#include <xkbcommon/xkbcommon.h>

#include <memory>

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class X11KeyboardGrabber;

class InputMethodKeyboardGrabV2 : public wl::server::ZwpInputMethodKeyboardGrabV2
{

    struct State
    {
        uint32_t modsDepressed;
        uint32_t modsLatched;
        uint32_t modsLocked;
        uint32_t group;
    };

    public : InputMethodKeyboardGrabV2(wl::server::Seat *seat);

    ~InputMethodKeyboardGrabV2();

    void sendKey(uint32_t keycode, bool isRelease);

protected:
    void resource_bind(wl::server::Resource *resource) override;
    void zwp_input_method_keyboard_grab_v2_release(wl::server::Resource *resource) override;

private:
    wl::server::Seat *seat_;
    std::unique_ptr<X11KeyboardGrabber> grabber_;

    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> xkbContext_;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> xkbKeymap_;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> xkbState_;

    State state_;

    std::pair<int, size_t> genKeymapData(xkb_keymap *keymap);
    bool updateState(uint32_t keycode, bool isRelease);
};

#endif // !INPUTMETHODKEYBOARDGRABV2_H

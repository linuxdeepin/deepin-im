// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_SERVER_ZWPINPUTMETHODKEYBOARDGRABV2_H
#define WL_SERVER_ZWPINPUTMETHODKEYBOARDGRABV2_H

#include "Type.h"
#include "wayland-input-method-unstable-v2-server-protocol.h"

namespace wl {
namespace server {

class ZwpInputMethodKeyboardGrabV2
    : public Type<ZwpInputMethodKeyboardGrabV2, zwp_input_method_keyboard_grab_v2>
{
    friend class Type;

public:
    ZwpInputMethodKeyboardGrabV2();
    virtual ~ZwpInputMethodKeyboardGrabV2();

    void send_keymap(struct wl_resource *resource, uint32_t format, int32_t fd, uint32_t size);
    void send_key(struct wl_resource *resource, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
    void send_modifiers(struct wl_resource *resource, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);

protected:
    virtual void zwp_input_method_keyboard_grab_v2_release(Resource *resource) = 0;

private:
    static const struct zwp_input_method_keyboard_grab_v2_interface impl;

};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPINPUTMETHODKEYBOARDGRABV2_H

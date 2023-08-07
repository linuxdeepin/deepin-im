// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_ZWPINPUTMETHODKEYBOARDGRABV2_H
#define WL_CLIENT_ZWPINPUTMETHODKEYBOARDGRABV2_H

#include "Type.h"

namespace wl {
namespace client {

class ZwpInputMethodKeyboardGrabV2 : public Type<zwp_input_method_keyboard_grab_v2>
{
public:
    ZwpInputMethodKeyboardGrabV2(zwp_input_method_keyboard_grab_v2 *val);
    ~ZwpInputMethodKeyboardGrabV2();
};

} // namespace client
}; // namespace wl

#endif // !WL_CLIENT_ZWPINPUTMETHODKEYBOARDGRABV2_H

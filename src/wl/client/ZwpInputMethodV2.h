// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_ZWPINPUTMETHODV2_H
#define WL_CLIENT_ZWPINPUTMETHODV2_H

#include "Type.h"

#include <memory>

namespace wl {
namespace client {

class ZwpInputMethodKeyboardGrabV2;

class ZwpInputMethodV2 : public Type<zwp_input_method_v2>
{
public:
    ZwpInputMethodV2(zwp_input_method_v2 *val);
    ~ZwpInputMethodV2();

    std::shared_ptr<ZwpInputMethodKeyboardGrabV2> grabKeyboard();

    void commit_string(const char *text);
    void set_preedit_string(const char *text, int32_t cursorBegin, int32_t cursorEnd);
    void commit(uint32_t serial);
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_ZWPINPUTMETHODV2_H

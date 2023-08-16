// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WL_CLIENT_ZWPDIMTEXTINPUTV3_H
#define WL_CLIENT_ZWPDIMTEXTINPUTV3_H

#include "Type.h"
#include "wayland-dim-text-input-unstable-v1-client-protocol.h"

namespace wl {
namespace client {

class ZwpDimTextInputV1 : public Type<zwp_dim_text_input_v1>
{
public:
    ZwpDimTextInputV1(zwp_dim_text_input_v1 *val);
    ~ZwpDimTextInputV1();

    void enable();
    void disable();
    void setSurroundingText(const char *text, int32_t cursor, int32_t anchor);
    void setTextChangeCause(uint32_t cause);
    void setContentType(uint32_t hint, uint32_t purpose);
    void setCursorRectangle(int32_t x, int32_t y, int32_t width, int32_t height);
    void commit();
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_ZWPDIMTEXTINPUTV3_H

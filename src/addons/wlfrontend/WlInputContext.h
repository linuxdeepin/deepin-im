// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WLINPUTCONTEXT_H
#define WLINPUTCONTEXT_H

#include "common/common.h"

#include <dimcore/InputContext.h>
#include <xkbcommon/xkbcommon.h>

#include <memory>

namespace org {
namespace deepin {
namespace dim {

class InputMethodV2;

class State
{
public:
    uint32_t serial = 1;
    uint32_t modifiers = 0;
    uint32_t group = 0;
};

class WlInputContext : public InputContext
{
    friend class InputMethodV2;
    friend class InputMethodKeyboardGrabV2;

public:
    WlInputContext(Dim *dim, InputMethodV2 *im);

public slots:
    void textInputRectangle(int32_t x, int32_t y, int32_t width, int32_t height);

private:
    InputMethodV2 *im_;

    std::unique_ptr<State> state_;

    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> xkbContext_;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> xkbKeymap_;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> xkbState_;

    uint32_t modifierMask_[static_cast<uint8_t>(Modifiers::CNT)];

    void keyEventDispatch();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !WLINPUTCONTEXT_H

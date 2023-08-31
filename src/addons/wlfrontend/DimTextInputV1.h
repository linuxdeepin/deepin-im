// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIMTEXTINPUTV1_H
#define DIMTEXTINPUTV1_H

#include "common/common.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"
#include "wl/client/ZwpInputMethodV2.h"
#include "wl/client/ZwpVirtualKeyboardV1.h"

#include <dimcore/InputContext.h>
#include <xkbcommon/xkbcommon.h>

#include <memory>

namespace org {
namespace deepin {
namespace dim {

class State
{
public:
    uint32_t serial = 1;
    uint32_t modifiers = 0;
    uint32_t group = 0;
};

class DimTextInputV1 : public InputContext
{
public:
    DimTextInputV1(Dim *dim,
                   const std::shared_ptr<wl::client::ZwpInputMethodV2> &im,
                   const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> &vk);

private:
    static const zwp_input_method_v2_listener imListener_;
    static const zwp_input_method_keyboard_grab_v2_listener grabListener_;
    const std::shared_ptr<wl::client::ZwpInputMethodV2> im_;
    const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> vk_;
    std::shared_ptr<wl::client::ZwpInputMethodKeyboardGrabV2> grab_;

    std::unique_ptr<State> state_;

    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> xkbContext_;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> xkbKeymap_;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> xkbState_;

    uint32_t modifierMask_[static_cast<uint8_t>(Modifiers::CNT)];

    void activate(struct zwp_input_method_v2 *zwp_input_method_v2);
    void deactivate(struct zwp_input_method_v2 *zwp_input_method_v2);
    void surroundingText(struct zwp_input_method_v2 *zwp_input_method_v2,
                         const char *text,
                         uint32_t cursor,
                         uint32_t anchor);
    void textChangeCause(struct zwp_input_method_v2 *zwp_input_method_v2, uint32_t cause);
    void contentType(struct zwp_input_method_v2 *zwp_input_method_v2,
                     uint32_t hint,
                     uint32_t purpose);
    void done(struct zwp_input_method_v2 *zwp_input_method_v2);
    void unavailable(struct zwp_input_method_v2 *zwp_input_method_v2);

    void keymap(struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
                uint32_t format,
                int32_t fd,
                uint32_t size);
    void key(struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
             uint32_t serial,
             uint32_t time,
             uint32_t key,
             uint32_t state);
    void modifiers(struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
                   uint32_t serial,
                   uint32_t mods_depressed,
                   uint32_t mods_latched,
                   uint32_t mods_locked,
                   uint32_t group);
    void repeatInfo(struct zwp_input_method_keyboard_grab_v2 *zwp_input_method_keyboard_grab_v2,
                    int32_t rate,
                    int32_t delay);
    void keyEventDispatch();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DIMTEXTINPUTV1_H

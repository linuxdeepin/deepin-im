#ifndef INPUTMETHODKEYBOARDV2_H
#define INPUTMETHODKEYBOARDV2_H

#include "common/common.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"
#include "wl/client/ZwpInputMethodKeyboardGrabV2.h"
#include "wl/client/ZwpInputMethodV2.h"
#include "wl/client/ZwpVirtualKeyboardV1.h"

#include <dimcore/InputContext.h>
#include <xkbcommon/xkbcommon.h>

#include <memory>

namespace org {
namespace deepin {
namespace dim {

class InputMethodKeyboardGrabV2 : public wl::client::ZwpInputMethodKeyboardGrabV2
{
public:
    InputMethodKeyboardGrabV2(Dim *dim,
                              const std::shared_ptr<wl::client::ZwpInputMethodV2> &im,
                              const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> &vk);

protected:
    void zwp_input_method_keyboard_grab_v2_keymap(uint32_t format, int32_t fd, uint32_t size) override;
    void zwp_input_method_keyboard_grab_v2_key(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) override;
    void zwp_input_method_keyboard_grab_v2_modifiers(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) override;
    void zwp_input_method_keyboard_grab_v2_repeat_info(int32_t rate, int32_t delay) override;

private:
    static const zwp_input_method_v2_listener imListener_;
    static const zwp_input_method_keyboard_grab_v2_listener grabListener_;
    const std::shared_ptr<wl::client::ZwpInputMethodV2> im_;
    const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> vk_;
    std::shared_ptr<wl::client::ZwpInputMethodKeyboardGrabV2> grab_;

    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> xkbContext_;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> xkbKeymap_;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> xkbState_;

    uint32_t modifierMask_[static_cast<uint8_t>(Modifiers::CNT)];
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHODKEYBOARDV2_H

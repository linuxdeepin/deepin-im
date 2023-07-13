#ifndef WAYLANDINPUTCONTEXTV1
#define WAYLANDINPUTCONTEXTV1

#include "WlType.h"
#include "common/common.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"

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

enum class Modifiers : uint8_t {
    Shift,
    Lock,
    Control,
    Mod1,
    Mod2,
    Mod3,
    Mod4,
    Mod5,
    Alt,
    Meta,
    Super,
    Hyper,

    CNT,
};

class WaylandInputContextV2 : public InputContext
{
public:
    WaylandInputContextV2(Dim *dim,
                          const std::shared_ptr<WlType<zwp_input_method_v2>> &im,
                          const std::shared_ptr<WlType<zwp_virtual_keyboard_v1>> &vk);

private:
    static const zwp_input_method_v2_listener im_listener_;
    static const zwp_input_method_keyboard_grab_v2_listener grab_listener_;
    std::shared_ptr<WlType<wl_seat>> seat_;
    std::shared_ptr<WlType<zwp_input_method_v2>> im_;
    std::shared_ptr<WlType<zwp_virtual_keyboard_v1>> vk_;
    std::shared_ptr<WlType<zwp_input_method_keyboard_grab_v2>> grab_;

    std::unique_ptr<State> state_;

    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> xkb_context_;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> xkb_keymap_;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> xkb_state_;

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
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !WAYLANDINPUTCONTEXTV1

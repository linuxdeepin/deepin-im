// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keyboard.h"

#include "Server.h"

extern "C" {
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_seat.h>
}

Keyboard::Keyboard(Server *server, wlr_seat *seat, wlr_input_device *device, wl_list *list)
    : server_(server)
    , keyboard_(wlr_keyboard_from_input_device(device))
    , modifiers_(this)
    , key_(this)
    , destroy_(this)
{
    /* We need to prepare an XKB keymap and assign it to the keyboard. This
     * assumes the defaults (e.g. layout = "us"). */
    struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    struct xkb_keymap *keymap =
        xkb_keymap_new_from_names(context, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);

    wlr_keyboard_set_keymap(keyboard_, keymap);
    xkb_keymap_unref(keymap);
    xkb_context_unref(context);
    wlr_keyboard_set_repeat_info(keyboard_, 25, 600);

    /* Here we set up listeners for keyboard events. */
    wl_signal_add(&keyboard_->events.modifiers, modifiers_);
    wl_signal_add(&keyboard_->events.key, key_);
    wl_signal_add(&device->events.destroy, destroy_);

    wlr_seat_set_keyboard(seat, keyboard_);

    /* And add the keyboard to our list of keyboards */
    wl_list_insert(list, &link_);
}

Keyboard::~Keyboard()
{
    wl_list_remove(&link_);
}

void Keyboard::modifiersNotify(void *data)
{
    /* This event is raised when a modifier key, such as shift or alt, is
     * pressed. We simply communicate this to the client. */
    /*
     * A seat can only have one keyboard, but this is a limitation of the
     * Wayland protocol - not wlroots. We assign all connected keyboards to the
     * same seat. You can swap out the underlying wlr_keyboard like this and
     * wlr_seat handles this transparently.
     */
    wlr_seat_set_keyboard(server_->seat(), keyboard_);
    /* Send modifiers to the client. */
    wlr_seat_keyboard_notify_modifiers(server_->seat(), &keyboard_->modifiers);
}

void Keyboard::keyNotify(void *data)
{
    /* This event is raised when a key is pressed or released. */
    struct wlr_keyboard_key_event *event = static_cast<wlr_keyboard_key_event *>(data);
    struct wlr_seat *seat = server_->seat();

    /* Translate libinput keycode -> xkbcommon */
    uint32_t keycode = event->keycode + 8;
    /* Get a list of keysyms based on the keymap for this keyboard */
    const xkb_keysym_t *syms;
    int nsyms = xkb_state_key_get_syms(keyboard_->xkb_state, keycode, &syms);

    bool handled = false;
    uint32_t modifiers = wlr_keyboard_get_modifiers(keyboard_);
    if ((modifiers & WLR_MODIFIER_ALT) && event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
        /* If alt is held down and this button was _pressed_, we attempt to
         * process it as a compositor keybinding. */
        for (int i = 0; i < nsyms; i++) {
            // handled = handle_keybinding(server, syms[i]);
            // todo
        }
    }

    if (!handled) {
        /* Otherwise, we pass it along to the client. */
        wlr_seat_set_keyboard(seat, keyboard_);
        wlr_seat_keyboard_notify_key(seat, event->time_msec, event->keycode, event->state);
    }
}

void Keyboard::destroyNotify(void *data)
{
    /* This event is raised by the keyboard base wlr_input_device to signal
     * the destruction of the wlr_keyboard. It will no longer receive events
     * and should be destroyed.
     */
    delete this;
}

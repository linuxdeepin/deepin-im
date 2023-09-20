// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODV2_H
#define INPUTMETHODV2_H

#include "wl/server/ZwpInputMethodV2.h"

#include <QtWaylandCompositor/QWaylandSeat>

#include <memory>

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class Seat;
class TextInputV3;
class InputMethodKeyboardGrabV2;

class InputMethodV2 : public wl::server::ZwpInputMethodV2
{
public:
    InputMethodV2(QWaylandSeat *seat);
    ~InputMethodV2();

    void sendDeactivate();
    void sendActivate();
    void sendSurroundingText(const char *text, int32_t cursor, int32_t anchor);
    void sendTextChangeCause(uint32_t cause);
    void sendContentType(uint32_t hint, uint32_t purpose);
    void sendDone();

protected:
    void zwp_input_method_v2_commit_string(wl::server::Resource *resource,
                                           const char *text) override;
    void zwp_input_method_v2_set_preedit_string(wl::server::Resource *resource,
                                                const char *text,
                                                int32_t cursor_begin,
                                                int32_t cursor_end) override;
    void zwp_input_method_v2_delete_surrounding_text(wl::server::Resource *resource,
                                                     uint32_t before_length,
                                                     uint32_t after_length) override;
    void zwp_input_method_v2_commit(wl::server::Resource *resource, uint32_t serial) override;
    void zwp_input_method_v2_get_input_popup_surface(wl::server::Resource *resource,
                                                     uint32_t id,
                                                     struct ::wl_resource *surface) override;
    void zwp_input_method_v2_grab_keyboard(wl::server::Resource *resource,
                                           uint32_t keyboard) override;
    void zwp_input_method_v2_destroy(wl::server::Resource *resource) override;

private:
    QWaylandSeat *seat_;

    std::unique_ptr<InputMethodKeyboardGrabV2> grab_;
};

#endif // !INPUTMETHODV2_H

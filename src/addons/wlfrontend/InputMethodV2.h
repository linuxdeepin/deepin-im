// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODV2_H
#define INPUTMETHODV2_H

#include "wl/client/ZwpInputMethodV2.h"

#include <QString>
#include <list>

namespace wl {
namespace client {
class ZwpVirtualKeyboardV1;
}
} // namespace wl

namespace org {
namespace deepin {
namespace dim {

class Dim;
class WlInputContext;
class InputMethodKeyboardGrabV2;

struct SurroundingText
{
    QString text;
    uint32_t cursor;
    uint32_t anchor;
};

struct TextChangeCause
{
    uint32_t cause;
};

struct ContentType
{
    uint32_t hint;
    uint32_t purpose;
};

class InputMethodV2 : public wl::client::ZwpInputMethodV2
{
    friend class InputMethodKeyboardGrabV2;
    friend class WlInputContext;

public:
    InputMethodV2(zwp_input_method_v2 *val,
                  Dim *dim,
                  const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> &vk);
    ~InputMethodV2() override;

protected:
    void zwp_input_method_v2_activate() override;
    void zwp_input_method_v2_deactivate() override;
    void zwp_input_method_v2_surrounding_text(const char *text,
                                              uint32_t cursor,
                                              uint32_t anchor) override;
    void zwp_input_method_v2_text_change_cause(uint32_t cause) override;
    void zwp_input_method_v2_content_type(uint32_t hint, uint32_t purpose) override;
    void zwp_input_method_v2_done() override;
    void zwp_input_method_v2_unavailable() override;

private:
    std::shared_ptr<InputMethodKeyboardGrabV2> grab_;
    std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> vk_;
    std::unique_ptr<WlInputContext> ic_;

    std::list<std::variant<SurroundingText, TextChangeCause, ContentType>> penddingEvents_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHODV2_H

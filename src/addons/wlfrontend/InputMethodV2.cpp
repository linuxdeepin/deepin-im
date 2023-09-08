// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodV2.h"

#include "InputMethodKeyboardGrabV2.h"
#include "WlInputContext.h"

#include <QDebug>

using namespace org::deepin::dim;

InputMethodV2::InputMethodV2(zwp_input_method_v2 *val,
                             Dim *dim,
                             const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> &vk)
    : wl::client::ZwpInputMethodV2(val)
    , vk_(vk)
    , ic_(std::make_unique<WlInputContext>(dim, this))
{
}

InputMethodV2::~InputMethodV2() = default;

void InputMethodV2::zwp_input_method_v2_activate()
{
    qDebug() << "im activated";

    grab_ = std::make_shared<InputMethodKeyboardGrabV2>(grabKeyboard(), this);

    ic_->focusIn();
}

void InputMethodV2::zwp_input_method_v2_deactivate()
{
    qDebug() << "im deactivated";

    ic_->state_.reset(new State);
    grab_.reset();

    ic_->focusOut();
}

void InputMethodV2::zwp_input_method_v2_surrounding_text(const char *text,
                                                         uint32_t cursor,
                                                         uint32_t anchor)
{
}

void InputMethodV2::zwp_input_method_v2_text_change_cause(uint32_t cause) { }

void InputMethodV2::zwp_input_method_v2_content_type(uint32_t hint, uint32_t purpose) { }

void InputMethodV2::zwp_input_method_v2_done() { }

void InputMethodV2::zwp_input_method_v2_unavailable() { }

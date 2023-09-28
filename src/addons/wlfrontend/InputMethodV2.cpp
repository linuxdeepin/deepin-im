// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputMethodV2.h"

#include "InputMethodKeyboardGrabV2.h"
#include "InputPopupSurfaceV2.h"
#include "WlInputContext.h"

#include <QDebug>

using namespace org::deepin::dim;

InputMethodV2::InputMethodV2(zwp_input_method_v2 *val,
                             const std::shared_ptr<wl::client::ZwpVirtualKeyboardV1> &vk,
                             const std::shared_ptr<wl::client::Surface> &surface,
                             Dim *dim)
    : wl::client::ZwpInputMethodV2(val)
    , vk_(vk)
    , surface_(surface)
    , popup_(std::make_shared<InputPopupSurfaceV2>(get_input_popup_surface(surface_)))
    , ic_(std::make_unique<WlInputContext>(dim, this))
{
    QObject::connect(popup_->getQObject(),
                     &InputPopupSurfaceV2QObj::textInputRectangle,
                     ic_.get(),
                     &WlInputContext::textInputRectangle);
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
    penddingEvents_.emplace_back(SurroundingText{ QString::fromUtf8(text), cursor, anchor });
}

void InputMethodV2::zwp_input_method_v2_text_change_cause(uint32_t cause)
{
    penddingEvents_.emplace_back(TextChangeCause{ cause });
}

void InputMethodV2::zwp_input_method_v2_content_type(uint32_t hint, uint32_t purpose)
{
    penddingEvents_.emplace_back(ContentType{ hint, purpose });
}

void InputMethodV2::zwp_input_method_v2_done()
{
    for (const auto &event : penddingEvents_) {
        if (std::holds_alternative<SurroundingText>(event)) {
            auto e = std::get<SurroundingText>(event);
            ic_->setSurroundingText(e.text, e.cursor, e.anchor);
        }
    }
}

void InputMethodV2::zwp_input_method_v2_unavailable() { }

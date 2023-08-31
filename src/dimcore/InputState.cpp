// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputState.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputState::InputState(Dim *dim)
    : dim_(dim)
    , ims_(dim_->imAddons())
{
    if (ims_.isEmpty()) {
        return;
    }

    connect(dim_, &Dim::imChanged, this, &InputState::handleImChanged);
    current_im_addon_ = ims_[0];
}

void InputState::handleImChanged()
{
    for (const auto im : ims_) {
        if (im == current_im_addon_ || im == "keyboard") {
            continue;
        }

        current_im_addon_ = im;
        break;
    }
}

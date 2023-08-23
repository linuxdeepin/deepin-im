// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputState.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputState::InputState(Dim *dim)
    : dim_(dim)
{
    auto ims = dim_->imAddons();
    if (ims.isEmpty()) {
        return;
    }
    current_im_addon_ = ims[0];
}

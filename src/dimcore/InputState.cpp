// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputState.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputState::InputState(Dim *dim)
    : dim_(dim)
    , ims_(dim_->imAddons())
    , currentImIdx_(-1)
{
    if (ims_.isEmpty()) {
        return;
    }

    currentImIdx_ = 0;
    currentImAddon_ = ims_[currentImIdx_];
}

void InputState::switchIMAddon()
{
    if (ims_.isEmpty()) {
        return;
    }

    auto nextIdx = currentImIdx_ + 1;
    if (nextIdx >= ims_.size()) {
        nextIdx = 0;
    }

    currentImIdx_ = nextIdx;
    currentImAddon_ = ims_[currentImIdx_];
}

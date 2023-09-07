// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputState.h"

#include "Dim.h"
#include "InputContext.h"

using namespace org::deepin::dim;

InputState::InputState(InputContext *ic)
    : ic_(ic)
    , ims_(ic_->dim_->imAddonNames())
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
    emit ic_->imAddonSwitched(currentImAddon_);
}

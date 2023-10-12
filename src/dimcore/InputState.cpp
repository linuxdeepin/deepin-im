// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputState.h"

#include "Dim.h"
#include "InputContext.h"
#include "InputMethodEntry.h"

using namespace org::deepin::dim;

InputState::InputState(InputContext *ic)
    : ic_(ic)
    , currentIMIndex_(std::make_pair("fcitx5proxy", "keyboard-us"))
{
    connect(ic_->dim_, &Dim::inputMethodEntryChanged, this, [this]() {
        const auto imEntry = ic_->dim()->imEntries()[currentIMIndex()];
        currentIMIndex_ = std::make_pair(imEntry.addonName(), imEntry.uniqueName());
    });
}

void InputState::switchIMAddon()
{
    auto idx = currentIMIndex();
    auto size = ic_->dim_->imEntries().size();

    ++idx;

    if (idx == size) {
        idx = 0;
    }

    const auto imEntry = ic_->dim()->imEntries()[idx];
    currentIMIndex_ = std::make_pair(imEntry.addonName(), imEntry.uniqueName());
    emit ic_->imSwitch(currentIMIndex_);
}

int InputState::currentIMIndex() const
{
    const auto &imList = ic_->dim_->imEntries();
    auto len = imList.size();

    for (int i = 0; i < len; ++i) {
        if (imList[i].uniqueName() == currentIMIndex_.second
            && imList[i].addonName() == currentIMIndex_.first) {
            return i;
        }
    }

    return 0;
}

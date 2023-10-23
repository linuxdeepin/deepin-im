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
    , currentIMKey_(std::make_pair("fcitx5proxy", "keyboard-us"))
{
    connect(ic_->dim_, &Dim::inputMethodEntryChanged, this, [this]() {
        auto iter = currentIMEntry();
        currentIMKey_ = std::make_pair(iter->addonName(), iter->uniqueName());
    });
}

void InputState::switchIMAddon()
{
    auto iter = currentIMEntry();
    iter++;

    const auto &imList = ic_->dim_->imEntries();
    if (iter == imList.cend()) {
        iter = imList.cbegin();
    }

    currentIMKey_ = std::make_pair(iter->addonName(), iter->uniqueName());
    emit ic_->imSwitch(currentIMKey_);
}

std::vector<InputMethodEntry>::const_iterator InputState::currentIMEntry() const
{
    const auto &imList = ic_->dim_->imEntries();

    auto iter = std::find_if(imList.cbegin(), imList.cend(), [this](const InputMethodEntry &entry) {
        return entry.addonName() == currentIMKey_.first
            && entry.uniqueName() == currentIMKey_.second;
    });
    if (iter == imList.cend()) {
        iter = imList.cbegin();
    }

    return iter;
}

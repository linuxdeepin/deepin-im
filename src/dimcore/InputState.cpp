// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InputState.h"

#include "Dim.h"
#include "InputContext.h"

using namespace org::deepin::dim;

InputState::InputState(InputContext *ic)
    : ic_(ic)
{
    connect(ic_->dim_, &Dim::inputMethodEntryChanged, this, [this]() {
        imEntries_ = ic_->dim_->imEntries();
        imEntryIt_ = imEntries_.begin();
    });
}

void InputState::switchIMAddon()
{
    if (imEntries_.empty()) {
        return;
    }

    ++imEntryIt_;
    if (imEntryIt_ == imEntries_.end()) {
        imEntryIt_ = imEntries_.begin();
    }

    const auto currentImName = imEntryIt_.value().uniqueName();

    emit ic_->imAddonSwitched(currentImName);
}

const InputMethodEntry &InputState::currentIMEntry() const
{
    return imEntryIt_.value();
}

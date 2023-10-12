// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualInputContextManager.h"

#include "AppMonitor.h"
#include "VirtualInputContext.h"
#include "VirtualInputContextGlue.h"
#include "dimcore/InputContext.h"

#include <experimental/unordered_map>

using namespace org::deepin::dim;

VirtualInputContextManager::VirtualInputContextManager(VirtualInputContextGlue *parentIC,
                                                       AppMonitor *appMonitor,
                                                       Dim *dim)
    : dim_(dim)
    , parentIC_(parentIC)
    , appMonitor_(appMonitor)
{
    parentIC_->setVirtualInputContextManager(this);

    connect(appMonitor_, &AppMonitor::appUpdated, this, &VirtualInputContextManager::appUpdated);
}

VirtualInputContextManager::~VirtualInputContextManager() = default;

void VirtualInputContextManager::setRealFocus(bool focus)
{
    parentIC_->setRealFocus(focus);
    updateFocus();
}

VirtualInputContext *VirtualInputContextManager::focusedVirtualIC()
{
    if (focus_.isEmpty()) {
        return nullptr;
    }

    return managed_.at(focus_).get();
}

void VirtualInputContextManager::appUpdated(const std::unordered_map<QString, pid_t> &appState,
                                            const QString &focus)
{
    std::experimental::erase_if(managed_, [&appState](const auto &item) {
        const auto &[key, pid] = item;
        return appState.find(key) == appState.end();
    });

    lastAppState_ = appState;
    focus_ = focus;
    updateFocus();
}

void VirtualInputContextManager::updateFocus()
{
    InputContext *ic = nullptr;
    if (!focus_.isEmpty()) {
        auto iter = managed_.find(focus_);
        if (iter != managed_.end()) {
            ic = iter->second.get();
        } else {
            auto result =
                managed_.emplace(focus_, std::make_unique<VirtualInputContext>(parentIC_, dim_));
            ic = result.first->second.get();
        }
    } else {
        ic = parentIC_;
    }

    if (parentIC_->realFocus()) {
        // forward capability flags on focus in.
        if (ic != parentIC_) {
            // ic->setCapabilityFlags(parentIC_->capabilityFlags());
            ic->surroundingText() = parentIC_->surroundingText();
            ic->updateSurroundingText();
        }
        ic->focusIn();
    } else {
        parentIC_->focusOut();
        for (const auto &[_, ic] : managed_) {
            ic->focusOut();
        }
    }
}

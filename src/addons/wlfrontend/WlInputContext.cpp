// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WlInputContext.h"

#include "InputMethodV2.h"
#include "common/common.h"
#include "dimcore/Dim.h"
#include "wl/client/ZwpVirtualKeyboardV1.h"

#include <linux/input.h>

#include <QDebug>

#include <sys/mman.h>

using namespace org::deepin::dim;

static int32_t getTimestamp()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / (1000 * 1000);
}

WlInputContext::WlInputContext(Dim *dim, InputMethodV2 *im)
    : InputContext(dim)
    , im_(im)
    , state_(std::make_unique<State>())
    , xkbContext_(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
{
    connect(this, &InputContext::processKeyEventFinished, this, [=]() {
        keyEventDispatch();
    });
}

WlInputContext::~WlInputContext() = default;

void WlInputContext::textInputRectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
    InputContextCursorRectChangeEvent e(this, x, y, width, height);
    dim()->postEvent(e);
}

void WlInputContext::keyEventDispatch()
{
    auto list = getAndClearBatchList();
    for (const auto &e : list) {
        if (std::holds_alternative<ForwardKey>(e)) {
            const auto &forwardKey = std::get<ForwardKey>(e);
            im_->vk_->key(getTimestamp(),
                          forwardKey.keycode + XKB_HISTORICAL_OFFSET,
                          forwardKey.pressed ? WL_KEYBOARD_KEY_STATE_PRESSED
                                             : WL_KEYBOARD_KEY_STATE_RELEASED);
            continue;
        }

        if (std::holds_alternative<PreeditInfo>(e)) {
            const auto &preeditInfo = std::get<PreeditInfo>(e);
            im_->set_preedit_string(preeditInfo.text.toStdString().c_str(),
                                    preeditInfo.cursorBegin,
                                    preeditInfo.cursorEnd);
            continue;
        }

        if (std::holds_alternative<CommitString>(e)) {
            const auto &text = std::get<CommitString>(e).text;
            im_->commit_string(text.toStdString().c_str());
            continue;
        }
    }

    if (!list.empty()) {
        im_->commit(state_->serial++);
    }
}

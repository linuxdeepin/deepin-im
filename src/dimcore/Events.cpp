// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Events.h"

using namespace org::deepin::dim;

Event::Event(EventType type, InputContext *ic)
    : type_(type)
    , ic_(ic)
{
}

InputContextKeyEvent::InputContextKeyEvent(InputContext *ic,
                                           uint32_t keyVal,
                                           uint32_t keycode,
                                           uint32_t state,
                                           bool isRelease,
                                           uint32_t time)
    : Event(EventType::InputContextKeyEvent, ic)
    , keyVal_(keyVal)
    , keycode_(keycode)
    , state_(state)
    , isRelease_(isRelease)
    , time_(time)
{
}

InputContextSetSurroundingTextEvent::InputContextSetSurroundingTextEvent(InputContext *ic,
    const QString &surroundingText, uint32_t cursor, uint32_t anchor)
    : Event(EventType::InputContextSetSurroundingText, ic)
    , text(surroundingText)
    , cursor(cursor)
    , anchor(anchor)
{
}

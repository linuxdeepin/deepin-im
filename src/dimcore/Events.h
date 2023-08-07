// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

namespace org {
namespace deepin {
namespace dim {

class InputContext;

enum class EventType {
    InputContextCreated,
    InputContextDestroyed,
    InputContextFocused,
    InputContextUnfocused,
    InputContextKeyEvent,
    InputContextCursorRectChanged,
};

enum ModifiersMask {
    SHIFT_MASK = 1 << 0,
    LOCK_MASK = 1 << 1,
    CONTROL_MASK = 1 << 2,
    MOD1_MASK = 1 << 3,
    MOD2_MASK = 1 << 4,
    MOD3_MASK = 1 << 5,
    MOD4_MASK = 1 << 6,
    MOD5_MASK = 1 << 7,
    ALT_MASK = 1 << 8,
    META_MASK = 1 << 9,
    SUPER_MASK = 1 << 10,
    HYPER_MASK = 1 << 11,
};

class Event
{
public:
    Event(EventType type_, InputContext *ic_);
    ~Event() = default;

    inline EventType type() const { return type_; }

    inline InputContext *ic() const { return ic_; }

private:
    EventType type_;
    InputContext *ic_;
};

class InputContextKeyEvent : public Event
{
public:
    InputContextKeyEvent(InputContext *ic,
                         uint32_t keyVal_,
                         uint32_t keycode,
                         uint32_t state,
                         bool isRelease,
                         uint32_t time);
    ~InputContextKeyEvent() = default;

public:
    inline uint32_t keyValue() const { return keyVal_; }

    inline uint32_t keycode() const { return keycode_; }

    inline uint32_t state() const { return state_; }

    inline bool isRelease() const { return isRelease_; }

    inline uint32_t time() const { return time_; }

private:
    uint32_t keyVal_;
    uint32_t keycode_;
    uint32_t state_;
    bool isRelease_;
    uint32_t time_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !EVENTS_H

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

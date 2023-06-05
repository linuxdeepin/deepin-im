#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

namespace org {
namespace deepin {
namespace dim {

class InputContext;

enum class EventType {
    Key,
};

class Event
{
public:
    Event(EventType type, InputContext *ic);
    ~Event() = default;

    EventType type;
    InputContext *ic;
};

class KeyEvent : public Event
{
public:
    KeyEvent(InputContext *ic,
             uint32_t keyval,
             uint32_t keycode,
             uint32_t state,
             bool isRelease,
             uint32_t time);
    ~KeyEvent() = default;

    uint32_t keyval;
    uint32_t keycode;
    uint32_t state;
    bool isRelease;
    uint32_t time;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !EVENTS_H

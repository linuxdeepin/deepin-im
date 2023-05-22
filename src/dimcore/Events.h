#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

enum class EventType {
    Key,
};

class Event {
public:
    Event(EventType type);
    ~Event() = default;

private:
    EventType type;
};

class KeyEvent : public Event {
public:
    KeyEvent(uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time);
    ~KeyEvent() = default;

    uint32_t keyval;
    uint32_t keycode;
    uint32_t state;
    bool isRelease;
    uint32_t time;
};

#endif // !EVENTS_H

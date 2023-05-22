#include "Events.h"

Event::Event(EventType type)
    : type(type) {
}

KeyEvent::KeyEvent(uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time)
    : Event(EventType::Key)
    , keyval(keyval)
    , keycode(keycode)
    , state(state)
    , isRelease(isRelease)
    , time(time) {
}

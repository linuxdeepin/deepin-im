#include "Events.h"

using namespace org::deepin::dim;

Event::Event(EventType type, InputContext *ic)
    : type(type)
    , ic(ic) {
}

KeyEvent::KeyEvent(InputContext *ic, uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time)
    : Event(EventType::Key, ic)
    , keyval(keyval)
    , keycode(keycode)
    , state(state)
    , isRelease(isRelease)
    , time(time) {
}

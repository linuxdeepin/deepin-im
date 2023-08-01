#include "InputMethodKeyboardGrabV2.h"

#include "X11KeyboardGrabber.h"
#include "qwayland-server-input-method-unstable-v2.h"

#include <QDateTime>
#include <QDebug>

class InputMethodKeyboardGrabV2Private : public QtWaylandServer::zwp_input_method_keyboard_grab_v2
{
public:
    InputMethodKeyboardGrabV2Private(InputMethodKeyboardGrabV2 *q)
        : q(q)
    {
    }

    ~InputMethodKeyboardGrabV2Private() { }

    void sendKey(uint32_t key, uint32_t state)
    {
        uint32_t ts = QDateTime::currentSecsSinceEpoch();
        const auto resources = resourceMap();
        for (auto r : resources) {
            send_key(r->handle, nextSerial(), ts, key, state);
        }
    }

protected:
    virtual void zwp_input_method_keyboard_grab_v2_release(Resource *resource) override
    {
        wl_resource_destroy(resource->handle);
    }

private:
    InputMethodKeyboardGrabV2 *q;
};

InputMethodKeyboardGrabV2::InputMethodKeyboardGrabV2(struct ::wl_resource *seat, QObject *parent)
    : QObject(parent)
    , d(new InputMethodKeyboardGrabV2Private(this))
    , m_seat(seat)
    , m_grabber(new X11KeyboardGrabber(this))
{
    connect(m_grabber,
            &X11KeyboardGrabber::keyEvent,
            this,
            &InputMethodKeyboardGrabV2::onX11KeyEvent);
}

InputMethodKeyboardGrabV2::~InputMethodKeyboardGrabV2() { }

INIT_FUNCS(InputMethodKeyboardGrabV2)

void InputMethodKeyboardGrabV2::onX11KeyEvent(int keycode, bool isRelease)
{
    d->sendKey(keycode, isRelease);
}

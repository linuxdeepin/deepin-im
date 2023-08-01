#ifndef INPUTMETHODKEYBOARDGRABV2_H
#define INPUTMETHODKEYBOARDGRABV2_H

#include "common.h"

#include <QObject>

struct wl_client;
struct wl_display;
struct wl_resource;

class InputMethodKeyboardGrabV2Private;
class X11KeyboardGrabber;

class InputMethodKeyboardGrabV2 : public QObject
{
    Q_OBJECT

    friend class InputMethodKeyboardGrabV2Private;

public:
    InputMethodKeyboardGrabV2(struct ::wl_resource *seat, QObject *parent);
    ~InputMethodKeyboardGrabV2();

    INIT_FUNCS_DEF

private:
    std::unique_ptr<InputMethodKeyboardGrabV2Private> d;
    struct ::wl_resource *m_seat;
    X11KeyboardGrabber *m_grabber;

    void onX11KeyEvent(int keycode, bool isRelease);
};

#endif // !INPUTMETHODKEYBOARDGRABV2_H

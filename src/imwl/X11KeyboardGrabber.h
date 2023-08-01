#ifndef X11KEYBOARDGRABBER_H
#define X11KEYBOARDGRABBER_H

#include "common.h"

#include <xcb/xcb.h>

#include <QObject>
#include <QSocketNotifier>

class X11KeyboardGrabber: public QObject
{
    Q_OBJECT
public:
    X11KeyboardGrabber(QObject *parent);
    ~X11KeyboardGrabber();

signals:
    void keyEvent(int keycode, bool isRelease);

private:
    std::unique_ptr<xcb_connection_t, Deleter<xcb_disconnect>> m_xconn;
    int m_xcbFd;
    QSocketNotifier *m_socketNotifier;
    const xcb_setup_t *m_setup;
    xcb_screen_t *m_screen;
    uint8_t m_xinput2OPCode;

    xcb_screen_t *screenOfDisplay(int screen);
    void onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent);
    void initXinputExtension();
};

#endif // !X11KEYBOARDGRABBER_H

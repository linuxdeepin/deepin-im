#ifndef INPUTCONTEXTPROXY_H
#define INPUTCONTEXTPROXY_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusServiceWatcher>

#include "InputmethodIface.h"
#include "InputcontextIface.h"

class InputContextProxy : public QObject {
public:
    InputContextProxy(QObject *parent = nullptr);

    void focusIn();
    void focusOut();
    void processKeyEvent(uint keyval, uint keycode, uint state, bool isRelease, uint time);

private:
    QDBusConnection bus_;
    QDBusServiceWatcher watcher_;
    org::deepin::dim::portal::inputmethod *im_;
    org::deepin::dim::portal::inputcontext *ic_;

private slots:
    void serviceAvailableChanged();
    void createIcFinished(QDBusPendingCallWatcher *watcher);
};

#endif // !INPUTCONTEXTPROXY_H

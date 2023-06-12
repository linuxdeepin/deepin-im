#ifndef INPUTCONTEXTPROXY_H
#define INPUTCONTEXTPROXY_H

#include "InputcontextIface.h"
#include "InputmethodIface.h"

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QObject>

struct BatchEvent
{
    uint32_t type;
    QVariant data;
};

Q_DECLARE_METATYPE(BatchEvent)

class InputContextProxy : public QObject
{
    Q_OBJECT

public:
    InputContextProxy(QObject *parent = nullptr);

    bool available() const { return available_; }

    void focusIn();
    void focusOut();
    void processKeyEvent(uint keyval, uint keycode, uint state, bool isRelease, uint time);

signals:
    void preeditString(const QString &text);
    void commitString(const QString &text);

private:
    QDBusConnection bus_;
    QDBusServiceWatcher watcher_;
    org::deepin::dim::portal::inputmethod *im_;
    org::deepin::dim::portal::inputcontext *ic_;

    bool available_;

private slots:
    void checkServiceAndCreateIC();
    void serviceAvailableChanged();
    void createIcFinished(QDBusPendingCallWatcher *watcher);
    void processKeyEventFinished(QDBusPendingCallWatcher *watcher);
};

#endif // !INPUTCONTEXTPROXY_H

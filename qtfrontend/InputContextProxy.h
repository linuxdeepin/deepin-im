#ifndef INPUTCONTEXTPROXY_H
#define INPUTCONTEXTPROXY_H

#include "InputcontextIface.h"
#include "InputmethodIface.h"
#include "utils/common.h"

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QObject>

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
    void preedit(const QList<QString> &preedit);
    void commitString(const QString &text);
    void forwardKey(uint32_t keyValue, uint32_t state, bool type);

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

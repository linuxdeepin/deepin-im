#include "InputContext1.h"

#include "dimcore/Events.h"

#include <QDBusConnection>

using namespace org::deepin::dim;

static const QString inputcontextPath = "/org/freedesktop/portal/inputcontext/%1";

enum { BATCHED_COMMIT_STRING = 0, BATCHED_PREEDIT, BATCHED_FORWARD_KEY };

InputContext1::InputContext1(Dim *dim, QObject *parent)
    : InputContext(dim, parent)
    , path_(inputcontextPath.arg(id()))
{
    QDBusConnection::sessionBus().registerObject(path_, this);
}

InputContext1::~InputContext1() { }

void InputContext1::updatePreedit(const PreeditKey &key)
{
    blockedEvents_.append(BatchEvent{ BATCHED_PREEDIT, QVariant::fromValue(key) });
}

void InputContext1::updateCommitString(const QString &text)
{
    Q_UNUSED(text);
    blockedEvents_.append(BatchEvent{ BATCHED_COMMIT_STRING, text });
}

void InputContext1::forwardKey(const ForwardKey &key)
{
    blockedEvents_.append(BatchEvent{ BATCHED_FORWARD_KEY, QVariant::fromValue(key) });
}

void InputContext1::FocusIn()
{
    focusIn();
}

void InputContext1::FocusOut()
{
    focusOut();
}

void InputContext1::Destroy()
{
    deleteLater();
}

QList<BatchEvent> InputContext1::ProcessKeyEvent(
    uint32_t keyval, uint32_t keycode, uint32_t state, bool isRelease, uint32_t time)
{
    InputContextKeyEvent e(this, keyval, keycode, state, isRelease, time);
    keyEvent(e);

    QList<BatchEvent> events;
    events.swap(blockedEvents_);
    return events;
}

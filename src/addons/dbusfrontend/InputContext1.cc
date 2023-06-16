#include "InputContext1.h"

#include "InputcontextAdaptor.h"
#include "dimcore/Events.h"

#include <QDBusConnection>

using namespace org::deepin::dim;

static const QString inputcontextPath = "/org/freedesktop/portal/inputcontext/%1";

enum { BATCHED_COMMIT_STRING = 0, BATCHED_PREEDIT, BATCHED_FORWARD_KEY };

InputContext1::InputContext1(Dim *dim, QObject *parent)
    : InputContext(dim, parent)
    , adaptor_(new InputcontextAdaptor(this))
    , path_(inputcontextPath.arg(id()))
{
    QDBusConnection::sessionBus().registerObject(path_, this);
}

InputContext1::~InputContext1() { }

void InputContext1::updatePreedit(const QList<QString> &preedit)
{
    blockedEvents_.append({ BATCHED_PREEDIT, QVariant::fromValue(preedit) });
}

void InputContext1::updateCommitString(const QString &text)
{
    blockedEvents_.append({ BATCHED_COMMIT_STRING, text });
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

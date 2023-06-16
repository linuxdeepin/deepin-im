#include "InputContextProxy.h"

#include <QDBusPendingCall>
#include <QList>

static const QString DIM_SERVICE = "org.deepin.dim";
static const QString DIM_IM_PATH = "/org/freedesktop/portal/inputmethod";

enum { BATCHED_COMMIT_STRING = 0, BATCHED_PREEDIT, BATCHED_FORWARD_KEY };

const QDBusArgument &operator>>(const QDBusArgument &argument, BatchEvent &event)
{
    argument.beginStructure();
    argument >> event.type >> event.data;
    argument.endStructure();
    return argument;
}

InputContextProxy::InputContextProxy(QObject *parent)
    : QObject(parent)
    , bus_(QDBusConnection::connectToBus(QDBusConnection::BusType::SessionBus, "dim"))
    , watcher_(DIM_SERVICE, bus_, QDBusServiceWatcher::WatchModeFlag::WatchForOwnerChange, this)
    , im_(new org::deepin::dim::portal::inputmethod(
          DIM_SERVICE, DIM_IM_PATH, QDBusConnection::sessionBus(), this))
    , ic_(nullptr)
    , available_(false)
{
    connect(&watcher_,
            &QDBusServiceWatcher::serviceOwnerChanged,
            this,
            &InputContextProxy::serviceAvailableChanged);
    checkServiceAndCreateIC();
}

void InputContextProxy::checkServiceAndCreateIC()
{
    if (!bus_.interface()->isServiceRegistered(DIM_SERVICE)) {
        return;
    }

    if (ic_) {
        available_ = false;
        delete ic_;
        ic_ = nullptr;
    }

    QDBusPendingReply<QDBusObjectPath> penddingReply = im_->CreateInputContext();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(penddingReply, this);
    QObject::connect(watcher,
                     &QDBusPendingCallWatcher::finished,
                     this,
                     &InputContextProxy::createIcFinished);
}

void InputContextProxy::serviceAvailableChanged()
{
    qDebug() << "onServiceOwnerChanged";
    QTimer::singleShot(100, this, &InputContextProxy::checkServiceAndCreateIC);
}

void InputContextProxy::createIcFinished(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (reply.isError()) {
        qDebug() << "CreateInputContext error:" << reply.error().message();
        return;
    }

    QDBusObjectPath path = reply.value();
    ic_ = new org::deepin::dim::portal::inputcontext(DIM_SERVICE, path.path(), bus_, this);
    available_ = true;
}

void InputContextProxy::focusIn()
{
    if (!ic_) {
        return;
    }

    ic_->FocusIn();
}

void InputContextProxy::focusOut()
{
    if (!ic_) {
        return;
    }

    ic_->FocusOut();
}

void InputContextProxy::processKeyEvent(
    uint keyval, uint keycode, uint state, bool isRelease, uint time)
{
    auto pendingCall = ic_->ProcessKeyEvent(keyval, keycode, state, isRelease, time);
    QDBusPendingCallWatcher watcher(pendingCall);
    connect(&watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            &InputContextProxy::processKeyEventFinished);
}

void InputContextProxy::processKeyEventFinished(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<QList<BatchEvent>> reply = *watcher;
    if (reply.isError()) {
        qDebug() << "ProcessKeyEvent error:" << reply.error().message();
        return;
    }

    QList<BatchEvent> events = reply.value();
    for (const auto &event : events) {
        switch (event.type) {
        case BATCHED_COMMIT_STRING: {
            emit commitString(event.data.toString());
            break;
        }
        case BATCHED_PREEDIT: {
            emit preeditString(event.data.toString());
            break;
        }
        case BATCHED_FORWARD_KEY: {
            ForwardKey data = event.data.value<ForwardKey>();
            emit forwardKey(data.keyValue, data.state, data.type);
            break;
        }
        default:
            qDebug() << "invalid event type " << event.type;
            return;
        }
    }
}

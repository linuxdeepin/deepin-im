// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "IBusProxy.h"

#include "dimcore/InputContext.h"
#include "ibustypes.h"

#ifndef IBUS_RELEASE_MASK
#  define IBUS_RELEASE_MASK (1 << 30)
#  define IBUS_SHIFT_MASK (1 << 0)
#  define IBUS_CONTROL_MASK (1 << 2)
#  define IBUS_MOD1_MASK (1 << 3)
#  define IBUS_META_MASK (1 << 28)
#endif

static const QString IBUS_PORTAL_SERVICE = QStringLiteral("org.freedesktop.portal.IBus");
static const QString IBUS_PORTAL_SERVICE_PATH = QStringLiteral("/org/freedesktop/IBus");

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(DimIBusProxy);

DimIBusProxy::DimIBusProxy(Dim *dim)
    : ProxyAddon(dim, "ibusproxy")
    , useSyncMode_(false)
    , dbusConn_(QDBusConnection::sessionBus())
    , serviceWatcher_(new QDBusServiceWatcher(
          IBUS_PORTAL_SERVICE, dbusConn_, QDBusServiceWatcher::WatchForOwnerChange, this))
{
    if (qEnvironmentVariableIsSet("IBUS_ENABLE_SYNC_MODE")) {
        bool ok;
        int enableSync = qEnvironmentVariableIntValue("IBUS_ENABLE_SYNC_MODE", &ok);
        if (ok && enableSync == 1)
            useSyncMode_ = true;
    }

    init();
}

QString DimIBusProxy::getSocketPath()
{
    QByteArray display;
    QByteArray displayNumber = "0";
    bool isWayland = false;

    if (qEnvironmentVariableIsSet("IBUS_ADDRESS_FILE")) {
        QByteArray path = qgetenv("IBUS_ADDRESS_FILE");
        return QString::fromLocal8Bit(path);
    } else if (qEnvironmentVariableIsSet("WAYLAND_DISPLAY")) {
        display = qgetenv("WAYLAND_DISPLAY");
        isWayland = true;
    } else {
        display = qgetenv("DISPLAY");
    }
    QByteArray host = "unix";

    if (isWayland) {
        displayNumber = display;
    } else {
        int pos = display.indexOf(':');
        if (pos > 0)
            host = display.left(pos);
        ++pos;
        int pos2 = display.indexOf('.', pos);
        displayNumber = pos2 > 0 ? display.mid(pos, pos2 - pos) : display.mid(pos);
    }

    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
        + QLatin1String("/ibus/bus/") + QLatin1String(QDBusConnection::localMachineId())
        + QLatin1Char('-') + QString::fromLocal8Bit(host) + QLatin1Char('-')
        + QString::fromLocal8Bit(displayNumber);
}

void DimIBusProxy::init()
{
    if (socketWatcher_.files().size() == 0) {
        QString socketPath = getSocketPath();
        QFile file(socketPath);
        if (file.open(QFile::ReadOnly)) {
            socketWatcher_.addPath(socketPath);
            connect(&socketWatcher_,
                    SIGNAL(fileChanged(QString)),
                    this,
                    SLOT(socketChanged(QString)));
        }
    }

    portalBus_ = new OrgFreedesktopIBusPortalInterface(IBUS_PORTAL_SERVICE,
                                                       IBUS_PORTAL_SERVICE_PATH,
                                                       dbusConn_,
                                                       this);

    timer_.setSingleShot(true);
    connect(&timer_, &QTimer::timeout, this, &DimIBusProxy::connectToBus);
    connect(serviceWatcher_,
            &QDBusServiceWatcher::serviceRegistered,
            this,
            &DimIBusProxy::busRegistered);
    connect(serviceWatcher_,
            &QDBusServiceWatcher::serviceUnregistered,
            this,
            &DimIBusProxy::busUnregistered);
}

void DimIBusProxy::connectToBus()
{
    init();
}

void DimIBusProxy::socketChanged(const QString &str)
{
    Q_UNUSED(str);

    timer_.stop();
    timer_.start(100);
}

void DimIBusProxy::busRegistered(const QString &str)
{
    Q_UNUSED(str);
    connectToBus();
}

void DimIBusProxy::busUnregistered(const QString &str)
{
    Q_UNUSED(str);
}

DimIBusProxy::~DimIBusProxy() { }

void DimIBusProxy::initInputMethods()
{
    Q_EMIT addonInitFinished(this);
}

QList<InputMethodEntry> DimIBusProxy::getInputMethods()
{
    return inputMethods_;
}

void DimIBusProxy::createFcitxInputContext(InputContext *ic)
{
    if (!ic || !portalBus_) {
        return;
    }

    auto id = ic->id();
    auto reply = portalBus_->CreateInputContext(QLatin1String("dim ibus context"));

    reply.waitForFinished();
    if (!reply.isValid()) {
        qWarning("QIBusPlatformInputContext: CreateInputContext failed.");
        return;
    }

    context_ = new OrgFreedesktopIBusInputContextInterface(IBUS_PORTAL_SERVICE,
                                                           reply.value().path(),
                                                           dbusConn_);

    if (!context_->isValid()) {
        qWarning("invalid input context");
        return;
    }

    iBusICMap_[id] = context_;

    enum Capabilities {
        IBUS_CAP_PREEDIT_TEXT = 1 << 0,
        IBUS_CAP_AUXILIARY_TEXT = 1 << 1,
        IBUS_CAP_LOOKUP_TABLE = 1 << 2,
        IBUS_CAP_FOCUS = 1 << 3,
        IBUS_CAP_PROPERTY = 1 << 4,
        IBUS_CAP_SURROUNDING_TEXT = 1 << 5
    };

    context_->SetCapabilities(IBUS_CAP_PREEDIT_TEXT | IBUS_CAP_FOCUS | IBUS_CAP_SURROUNDING_TEXT);

    connect(context_,
            &OrgFreedesktopIBusInputContextInterface::CommitText,
            this,
            [ic](const QDBusVariant &text) {
                const QDBusArgument arg = qvariant_cast<QDBusArgument>(text.variant());
                IBusText t;
                arg >> t;

                ic->updateCommitString(t.text);
            });
    connect(context_,
            &OrgFreedesktopIBusInputContextInterface::UpdatePreeditText,
            this,
            [ic](const QDBusVariant &text, uint cursorPos, bool visible) {
                const QDBusArgument arg = qvariant_cast<QDBusArgument>(text.variant());

                IBusText t;
                arg >> t;

                ic->updatePreedit(t.text, cursorPos, cursorPos);
            });
    connect(context_,
            &OrgFreedesktopIBusInputContextInterface::ForwardKeyEvent,
            this,
            [ic](uint keyval, uint keycode, uint state) {
                ic->forwardKey(keycode, state);
            });
}

void DimIBusProxy::focusIn(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        iBusICMap_[id]->FocusIn();
    }
}

void DimIBusProxy::focusOut(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        iBusICMap_[id]->FocusOut();
    }
}

void DimIBusProxy::destroyed(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        iBusICMap_[id]->Destroy();
    }
}

bool DimIBusProxy::keyEvent(InputContextKeyEvent &keyEvent)
{
    bool result = false;

    quint32 ibusState = keyEvent.state();

    if (keyEvent.isRelease())
        ibusState |= IBUS_RELEASE_MASK;

    auto id = keyEvent.ic()->id();

    if (isICDBusInterfaceValid(id)) {
        keyEvent.ic()->setAsyncMode(true);
        auto reply =
            iBusICMap_[id]->ProcessKeyEvent(keyEvent.keyValue(), keyEvent.keycode(), ibusState);
        reply.waitForFinished();
        if (useSyncMode_ || reply.isFinished()) {
            result = reply.value();
        }
    }

    return result;
}

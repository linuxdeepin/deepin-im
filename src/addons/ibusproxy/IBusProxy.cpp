// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "IBusProxy.h"

#include "IBUSIface.h"
#include "IBUSPortIface.h"
#include "dimcore/Dim.h"
#include "dimcore/InputContext.h"
#include "ibustypes.h"

#include <signal.h>

#ifndef IBUS_RELEASE_MASK
#  define IBUS_RELEASE_MASK (1 << 30)
#  define IBUS_SHIFT_MASK (1 << 0)
#  define IBUS_CONTROL_MASK (1 << 2)
#  define IBUS_MOD1_MASK (1 << 3)
#  define IBUS_META_MASK (1 << 28)
#endif

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(DimIBusProxy);

class DimIBusInputContextPrivate
{
public:
    DimIBusInputContextPrivate();

    ~DimIBusInputContextPrivate()
    {
        delete context_;
        delete busInterface_;
        delete portalBus_;
        delete dbusConn_;
    }

    static QString getSocketPath();
    QDBusConnection *createConnection();
    void initBus();
    void createBusProxy();

    bool usePortal_;
    bool isValid_;
    bool busConnected_;
    QString ibusService_;
    QDBusConnection *dbusConn_;
    OrgFreedesktopIBusInputContextInterface *context_;
    OrgFreedesktopIBusPortalInterface *portalBus_;
    OrgFreedesktopIBusInterface *busInterface_;
    QDBusServiceWatcher serviceWatcher_;
};

DimIBusInputContextPrivate::DimIBusInputContextPrivate()
    : usePortal_(false)
    , isValid_(false)
    , busConnected_(false)
    , dbusConn_(nullptr)
    , context_(nullptr)
    , portalBus_(nullptr)
    , busInterface_(nullptr)
{
    if (qEnvironmentVariableIsSet("IBUS_USE_PORTAL")) {
        bool ok;
        int enableSync = qEnvironmentVariableIntValue("IBUS_USE_PORTAL", &ok);
        if (ok && enableSync == 1)
            usePortal_ = true;
    }
    if (usePortal_) {
        isValid_ = true;
    } else {
        isValid_ =
            !QStandardPaths::findExecutable(QString::fromLocal8Bit("ibus-daemon"), QStringList())
                 .isEmpty();
    }
    if (!isValid_)
        return;
}

void DimIBusInputContextPrivate::initBus()
{
    dbusConn_ = createConnection();
    createBusProxy();
}

void DimIBusInputContextPrivate::createBusProxy()
{
    if (!dbusConn_ || !dbusConn_->isConnected())
        return;

    ibusService_ = usePortal_ ? QLatin1String("org.freedesktop.portal.IBus")
                              : QLatin1String("org.freedesktop.IBus");
    if (usePortal_) {
        portalBus_ = new OrgFreedesktopIBusPortalInterface((ibusService_),
                                                           QLatin1String("/org/freedesktop/IBus"),
                                                           *dbusConn_);
        if (!portalBus_->isValid()) {
            qWarning("ibus proxy: invalid portal bus");
            return;
        }
    } else {
        busInterface_ = new OrgFreedesktopIBusInterface(ibusService_,
                                                        QLatin1String("/org/freedesktop/IBus"),
                                                        *dbusConn_);
        if (!busInterface_->isValid()) {
            qWarning("ibus proxy: invalid bus.");
            return;
        }
    }

    serviceWatcher_.removeWatchedService(ibusService_);
    serviceWatcher_.setConnection(*dbusConn_);
    serviceWatcher_.addWatchedService(ibusService_);

    busConnected_ = true;
}

QString DimIBusInputContextPrivate::getSocketPath()
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

QDBusConnection *DimIBusInputContextPrivate::createConnection()
{
    if (usePortal_)
        return new QDBusConnection(QDBusConnection::connectToBus(QDBusConnection::SessionBus,
                                                                 QLatin1String("DimIBusProxy")));
    QFile file(getSocketPath());

    if (!file.open(QFile::ReadOnly))
        return 0;

    QByteArray address;
    int pid = -1;

    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if (line.startsWith('#'))
            continue;

        if (line.startsWith("IBUS_ADDRESS="))
            address = line.mid(sizeof("IBUS_ADDRESS=") - 1);
        if (line.startsWith("IBUS_DAEMON_PID="))
            pid = line.mid(sizeof("IBUS_DAEMON_PID=") - 1).toInt();
    }

    if (address.isEmpty() || pid < 0 || kill(pid, 0) != 0)
        return 0;

    return new QDBusConnection(
        QDBusConnection::connectToBus(QString::fromLatin1(address), QLatin1String("DimIBusProxy")));
}

DimIBusProxy::DimIBusProxy(Dim *dim)
    : ProxyAddon(dim, "ibusproxy", "ibus")
    , d(new DimIBusInputContextPrivate())
    , useSyncMode_(false)
{
    qDBusRegisterMetaType<IBusText>();
    qDBusRegisterMetaType<IBusEngineDesc>();

    if (qEnvironmentVariableIsSet("IBUS_ENABLE_SYNC_MODE")) {
        bool ok;
        int enableSync = qEnvironmentVariableIntValue("IBUS_ENABLE_SYNC_MODE", &ok);
        if (ok && enableSync == 1)
            useSyncMode_ = true;
    }

    if (!d->usePortal_) {
        QString socketPath = DimIBusInputContextPrivate::getSocketPath();
        QFile file(socketPath);
        if (file.open(QFile::ReadOnly)) {
            qDebug() << "socketWatcher.addPath" << socketPath;
            // If restart ibus-daemon,
            // the applications could run before ibus-daemon runs.
            // We watch the getSocketPath() to get the launching ibus-daemon.
            socketWatcher_.addPath(socketPath);
            connect(&socketWatcher_,
                    &QFileSystemWatcher::fileChanged,
                    this,
                    &DimIBusProxy::socketChanged);
        }
        timer_.setSingleShot(true);
        connect(&timer_, &QTimer::timeout, this, &DimIBusProxy::connectToBus);
    }

    connect(&d->serviceWatcher_,
            &QDBusServiceWatcher::serviceRegistered,
            this,
            &DimIBusProxy::busRegistered);
    connect(&d->serviceWatcher_,
            &QDBusServiceWatcher::serviceUnregistered,
            this,
            &DimIBusProxy::busUnregistered);
}

void DimIBusProxy::connectToBus()
{
    qDebug() << "connect to ibus";

    d->initBus();
    initEngines();

    if (!d->usePortal_ && socketWatcher_.files().size() == 0)
        socketWatcher_.addPath(DimIBusInputContextPrivate::getSocketPath());

    const auto &inputContexts = dim()->getInputContexts();
    for (auto i = inputContexts.begin(); i != inputContexts.end(); ++i) {
        createFcitxInputContext(i->second);
    }
}

void DimIBusProxy::socketChanged(const QString &str)
{
    Q_UNUSED(str);
    qDebug() << "socketChanged";

    if (d->context_)
        disconnect(d->context_);
    if (d->busInterface_ && d->busInterface_->isValid())
        disconnect(d->busInterface_);
    if (d->dbusConn_)
        d->dbusConn_->disconnectFromBus(QLatin1String("DimIBusProxy"));

    timer_.stop();
    timer_.start(100);
}

void DimIBusProxy::busRegistered(const QString &str)
{
    qDebug() << "bus registered";
    Q_UNUSED(str);
    if (d->usePortal_) {
        connectToBus();
    }
}

void DimIBusProxy::busUnregistered(const QString &str)
{
    qDebug() << "bus unregistered";
    Q_UNUSED(str);
    d->busConnected_ = false;
}

DimIBusProxy::~DimIBusProxy()
{
    iBusICMap_.clear();
}

void DimIBusProxy::initInputMethods()
{
    Q_EMIT addonInitFinished(this);
}

const QList<InputMethodEntry> &DimIBusProxy::getInputMethods()
{
    return inputMethods_;
}

void DimIBusProxy::createFcitxInputContext(InputContext *ic)
{
    if (!d->busConnected_ || !ic) {
        return;
    }

    auto id = ic->id();

    const auto &icName = QString("DimInputContext-%1").arg(id);
    const auto &icDbusObj = d->usePortal_ ? d->portalBus_->CreateInputContext(icName)
                                          : d->busInterface_->CreateInputContext(icName);
    if (!icDbusObj.isValid()) {
        qWarning("ibus proxy: CreateInputContext failed.");
        return;
    }

    const auto &ibusIcPath = icDbusObj.value().path();

    d->context_ =
        new OrgFreedesktopIBusInputContextInterface(d->ibusService_, ibusIcPath, *d->dbusConn_);

    if (!d->context_->isValid()) {
        qWarning("invalid input context");
        return;
    }

    iBusICMap_[id] = d->context_;

    enum Capabilities {
        IBUS_CAP_PREEDIT_TEXT = 1 << 0,
        IBUS_CAP_AUXILIARY_TEXT = 1 << 1,
        IBUS_CAP_LOOKUP_TABLE = 1 << 2,
        IBUS_CAP_FOCUS = 1 << 3,
        IBUS_CAP_PROPERTY = 1 << 4,
        IBUS_CAP_SURROUNDING_TEXT = 1 << 5
    };

    d->context_->SetCapabilities(IBUS_CAP_PREEDIT_TEXT | IBUS_CAP_FOCUS
                                 | IBUS_CAP_SURROUNDING_TEXT);

    connect(d->context_,
            &OrgFreedesktopIBusInputContextInterface::CommitText,
            this,
            [ic](const QDBusVariant &text) {
                const QDBusArgument arg = qvariant_cast<QDBusArgument>(text.variant());
                IBusText t;
                arg >> t;

                ic->commitString(t.text);
            });
    connect(d->context_,
            &OrgFreedesktopIBusInputContextInterface::UpdatePreeditText,
            this,
            [ic](const QDBusVariant &text, uint cursorPos, bool visible) {
                const QDBusArgument arg = qvariant_cast<QDBusArgument>(text.variant());

                IBusText t;
                arg >> t;

                ic->updatePreedit(t.text, cursorPos, cursorPos);
            });
    connect(d->context_,
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

void DimIBusProxy::setCurrentIM(const QString &im)
{
    if (!d->busConnected_ || d->usePortal_ || !d->busInterface_) {
        return;
    }

    QDBusPendingReply<> reply = d->busInterface_->SetGlobalEngine(im);
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "set global engine error" << reply.error();
    }
}

bool DimIBusProxy::keyEvent([[maybe_unused]] const InputMethodEntry &entry,
                            InputContextKeyEvent &keyEvent)
{
    bool result = false;

    quint32 ibusState = keyEvent.state();

    if (keyEvent.isRelease())
        ibusState |= IBUS_RELEASE_MASK;

    auto id = keyEvent.ic()->id();

    if (isICDBusInterfaceValid(id)) {
        auto reply =
            iBusICMap_[id]->ProcessKeyEvent(keyEvent.keyValue(), keyEvent.keycode(), ibusState);
        reply.waitForFinished();

        if (reply.isError()) {
            qWarning() << "processKeyEvent error: " << reply.error().message();
            return false;
        }

        if (useSyncMode_ || reply.isFinished()) {
            result = reply.value();
        }
    }

    return result;
}

void DimIBusProxy::cursorRectangleChangeEvent(InputContextCursorRectChangeEvent &event)
{
    auto id = event.ic()->id();

    if (!isICDBusInterfaceValid(id)) {
        return;
    }

    iBusICMap_[id]->SetCursorLocationRelative(event.x, event.y, event.w, event.h);
}

void DimIBusProxy::updateSurroundingText(Event &event)
{
    auto id = event.ic()->id();
    if (!isICDBusInterfaceValid(id)) {
        return;
    }

    IBusText text;
    auto &surroundingText = event.ic()->surroundingText();
    text.text = surroundingText.text();

    QVariant variant;
    variant.setValue(text);
    QDBusVariant dbusText(variant);

    iBusICMap_[id]->SetSurroundingText(dbusText,
                                       surroundingText.cursor(),
                                       surroundingText.anchor());
}

void DimIBusProxy::initEngines()
{
    QList<InputMethodEntry> inputMethods;
    for (auto &engine : listEngines()) {
        QString imEntryName = engine.engine_name;

        // 过滤掉键盘布局
        if (imEntryName.startsWith(QLatin1String("xkb:"))) {
            continue;
        }

        inputMethods.append(InputMethodEntry(key(),
                                             imEntryName,
                                             engine.longname,
                                             engine.description,
                                             engine.symbol,
                                             engine.icon));
    }

    inputMethods_.swap(inputMethods);
    Q_EMIT addonInitFinished(this);
}

void DimIBusProxy::globalEngineChanged(const QString &engineName)
{
    if (d->usePortal_ || !d->busConnected_) {
        return;
    }

    initEngines();
}

QList<IBusEngineDesc> DimIBusProxy::listEngines()
{
    QList<IBusEngineDesc> engines;

    if (d->usePortal_) {
        return engines;
    }

    if (!d->busConnected_) {
        qWarning() << "IBus is not connected!";
        return engines;
    }

    QDBusPendingReply<QVariantList> reply = d->busInterface_->ListEngines();
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "Bus::listEngines:" << reply.error();
        return engines;
    }

    QVariantList ret = reply.value();
    for (int i = 0; i < ret.size(); i++) {
        IBusEngineDesc e;
        ret.at(i).value<QDBusArgument>() >> e;
        engines << e;
    }

    return engines;
}

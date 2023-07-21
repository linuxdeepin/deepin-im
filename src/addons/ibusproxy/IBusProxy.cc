#include "IBusProxy.h"

#include "dimcore/InputContext.h"

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(DimIBusProxy);

DimIBusProxy::DimIBusProxy(Dim *dim)
    : ProxyAddon(dim, "ibusproxy")
    , m_watcher(new QDBusServiceWatcher(this))
{
    m_watcher->setConnection(QDBusConnection::sessionBus());
    m_watcher->addWatchedService(QStringLiteral("org.freedesktop.IBus"));

    init();
    updateInputMethods();
}

static void ibus_connected_cb(IBusBus *m_bus, gpointer user_data)
{
    Q_UNUSED(m_bus);
    DimIBusProxy *proxy = (DimIBusProxy *)user_data;
    proxy->init();
}

static void ibus_disconnected_cb(IBusBus *m_bus, gpointer user_data)
{
    Q_UNUSED(m_bus);
    DimIBusProxy *proxy = (DimIBusProxy *)user_data;
    proxy->finalize();
}

void DimIBusProxy::init()
{
    ibus_init();

    m_bus = ibus_bus_new();

    if (!ibus_bus_is_connected(m_bus)) {
        return;
    }
    g_signal_connect(m_bus, "disconnected", G_CALLBACK(ibus_disconnected_cb), this);
    g_signal_connect(m_bus, "connected", G_CALLBACK(ibus_connected_cb), this);
    connect(m_watcher, &QDBusServiceWatcher::serviceUnregistered, this, &DimIBusProxy::finalize);
}

void DimIBusProxy::finalize()
{
    clean();
    destroyed(0);
}

void DimIBusProxy::clean()
{
    if (m_bus) {
        g_signal_handlers_disconnect_by_func(m_bus, (gpointer)ibus_disconnected_cb, this);
        g_signal_handlers_disconnect_by_func(m_bus, (gpointer)ibus_connected_cb, this);
        g_object_unref(m_bus);
        m_bus = nullptr;
    }
}

DimIBusProxy::~DimIBusProxy()
{
    clean();
}

QList<InputMethodEntry> DimIBusProxy::getInputMethods()
{
    return inputMethods_;
}

void DimIBusProxy::createFcitxInputContext(InputContext *ic)
{
    QDBusInterface interface = QDBusInterface(IBUS_SERVICE_PORTAL,
                                              IBUS_PATH_IBUS,
                                              IBUS_INTERFACE_PORTAL,
                                              m_watcher->connection());

    // TODO: it must be actual app name
    QDBusPendingCall call = interface.asyncCall("CreateInputContext", "dim");

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    QObject::connect(
        watcher,
        &QDBusPendingCallWatcher::finished,
        this,
        [this, ic](QDBusPendingCallWatcher *watcher) {
            watcher->deleteLater();
            QDBusPendingReply<QDBusObjectPath> reply = *watcher;
            if (reply.isError()) {
                qDebug() << "create ibus input context error:" << reply.error().message();
                return;
            }

            const QString path = reply.value().path();

            auto *icIface = new org::freedesktop::IBus::InputContext(IBUS_SERVICE_PORTAL,
                                                                     path,
                                                                     QDBusConnection::sessionBus(),
                                                                     this);
            if (icIface && icIface->isValid()) {
                iBusICInterfaceMap_[ic->id()] = icIface;
            }

            GDBusConnection *connection = ibus_bus_get_connection(m_bus);
            if (connection) {
                auto context =
                    ibus_input_context_get_input_context(path.toStdString().c_str(), connection);

                if (context) {
                    iBusICMap_[ic->id()] = context;
                }
            }
        });
}

void DimIBusProxy::focusIn(uint32_t id)
{
    if (isIBusICValid(id)) {
        ibus_input_context_focus_in(iBusICMap_[id]);
    }
}

void DimIBusProxy::focusOut(uint32_t id)
{
    if (isIBusICValid(id)) {
        ibus_input_context_focus_out(iBusICMap_[id]);
    }
}

void DimIBusProxy::destroyed(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        QDBusInterface interface = QDBusInterface(IBUS_SERVICE_PORTAL,
                                                  iBusICInterfaceMap_[id]->path(),
                                                  "org.freedesktop.IBus.Service",
                                                  QDBusConnection::sessionBus());

        QDBusPendingCall call = interface.asyncCall("Destroy");
        call.waitForFinished();

        if (call.isError()) {
            qWarning() << "destroyed" << id << iBusICInterfaceMap_[id]->path();
        }
    }
}

bool DimIBusProxy::keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent)
{
    Q_UNUSED(entry);
    Q_UNUSED(keyEvent);

    return false;
}

void DimIBusProxy::updateInputMethods() { }

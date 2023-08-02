#include "IBusProxy.h"

#include "dimcore/InputContext.h"

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(DimIBusProxy);

DimIBusProxy::DimIBusProxy(Dim *dim)
    : ProxyAddon(dim, "ibusproxy")
    , init_(false)
{
    init();
}

bool DimIBusProxy::isUseSyncMode()
{
    auto mode = qgetenv("IBUS_ENABLE_SYNC_MODE");
    if (mode.compare("1") == 0 || mode.compare("true") == 0) {
        return true;
    }

    // use sync mode default
    return true;
}

static void iBusConnected(IBusBus *bus_, gpointer user_data)
{
    Q_UNUSED(bus_);

    g_return_if_fail(user_data);
    DimIBusProxy *proxy = (DimIBusProxy *)user_data;
    proxy->init();
}

static void iBusDisconnected(IBusBus *bus_, gpointer user_data)
{
    Q_UNUSED(bus_);

    g_return_if_fail(user_data);
    DimIBusProxy *proxy = (DimIBusProxy *)user_data;
    proxy->finalize();
}

static void iBusGlobalEngineChanged(IBusBus *bus, const gchar *engine_name, gpointer user_data)
{
    Q_UNUSED(bus);

    qDebug() << "Global engine is changed to " << engine_name;
    g_return_if_fail(user_data);

    DimIBusProxy *proxy = (DimIBusProxy *)user_data;
    proxy->updateInputMethods();
}

static void processKeyEventDone(GObject *object, GAsyncResult *res, gpointer user_data)
{
    g_return_if_fail(user_data);
    IBusInputContext *context = (IBusInputContext *)object;

    GError *error = nullptr;
    gboolean retval = ibus_input_context_process_key_event_async_finish(context, res, &error);

    if (error != nullptr) {
        qWarning() << "Process Key Event failed" << error->message;
        g_error_free(error);
    }

    if (retval == FALSE) {
        // TODO: need to ignore the event and return
    }
}

static void commitText(IBusInputContext *context, IBusText *text, InputContext *ic)
{
    g_assert(IBUS_IS_INPUT_CONTEXT(context));
    g_assert(IBUS_IS_TEXT(text));

    ic->updateCommitString(text->text);
}

static void forwardKeyEvent(
    IBusInputContext *context, guint keyval, guint keycode, guint state, InputContext *ic)
{
    Q_UNUSED(context);
    Q_UNUSED(keyval);
    ic->forwardKey(keycode, state);
}

static void updatePreedit(
    IBusInputContext *context, IBusText *text, gint cursor_pos, gboolean visible, InputContext *ic)
{
    Q_UNUSED(visible);
    g_assert(IBUS_IS_INPUT_CONTEXT(context));
    g_assert(IBUS_IS_TEXT(text));

    ic->updatePreedit(text->text, cursor_pos, cursor_pos);
}

static void showPreedit(IBusInputContext *context)
{
    g_assert(IBUS_IS_INPUT_CONTEXT(context));
}

void DimIBusProxy::init()
{
    // only init once
    if (init_) {
        return;
    }

    ibus_init();

    bus_ = ibus_bus_new();
    if (!bus_) {
        qWarning() << "create ibus failed";
        return;
    }

    g_signal_connect(bus_, "connected", G_CALLBACK(iBusConnected), this);
    g_signal_connect(bus_, "disconnected", G_CALLBACK(iBusDisconnected), this);
    g_signal_connect(bus_, "global-engine-changed", G_CALLBACK(iBusGlobalEngineChanged), this);

    ibus_bus_set_watch_dbus_signal(bus_, TRUE);
    ibus_bus_set_watch_ibus_signal(bus_, TRUE);

    if (!ibus_bus_is_connected(bus_)) {
        return;
    }

    init_ = true;
}

void DimIBusProxy::finalize()
{
    clean();
    destroyed(0);
}

void DimIBusProxy::clean()
{
    if (bus_) {
        g_signal_handlers_disconnect_by_func(bus_, (gpointer)iBusDisconnected, this);
        g_signal_handlers_disconnect_by_func(bus_, (gpointer)iBusConnected, this);
        g_object_unref(bus_);
        bus_ = nullptr;
    }
}

DimIBusProxy::~DimIBusProxy()
{
    clean();

    for (auto iter = iBusICMap_.begin(); iter != iBusICMap_.end(); ++iter) {
        auto v = iter.value();
        if (v != nullptr) {
            g_slice_free(IBusInputContext, v);
            v = nullptr;
        }
    }
}

void DimIBusProxy::initInputMethods()
{
    updateInputMethods();
    Q_EMIT addonInitFinished(this);
}

QList<InputMethodEntry> DimIBusProxy::getInputMethods()
{
    return inputMethods_;
}

void DimIBusProxy::createFcitxInputContext(InputContext *ic)
{
    if (!ic || !bus_) {
        return;
    }

    auto id = ic->id();

    QString name = QString("%1_%2").arg("dim").arg(id);

    IBusInputContext *ibusIC = ibus_bus_create_input_context(bus_, name.toStdString().c_str());

    if (ibusIC == nullptr) {
        g_slice_free(IBusInputContext, ibusIC);
        return;
    }

    iBusICMap_[id] = ibusIC;

    g_signal_connect(ibusIC, "commit-text", G_CALLBACK(commitText), ic);
    g_signal_connect(ibusIC, "forward-key-event", G_CALLBACK(forwardKeyEvent), ic);
    g_signal_connect(ibusIC, "update-preedit-text", G_CALLBACK(updatePreedit), ic);
    g_signal_connect(ibusIC, "show-preedit-text", G_CALLBACK(showPreedit), ic);

    ibus_input_context_set_capabilities(ibusIC, IBUS_CAP_FOCUS | IBUS_CAP_PREEDIT_TEXT);
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
            qWarning() << "failed to destroyed ibus inputContext" << id
                       << iBusICInterfaceMap_[id]->path();
        }
    }
}

bool DimIBusProxy::keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent)
{
    Q_UNUSED(entry);
    bool result = false;

    auto id = keyEvent.ic()->id();

    // only handle key press
    if (keyEvent.isRelease() == true) {
        return false;
    }

    if (isIBusICValid(id)) {
        if (isUseSyncMode()) {
            result = ibus_input_context_process_key_event(iBusICMap_[id],
                                                          keyEvent.keyValue(),
                                                          keyEvent.keycode() - 8,
                                                          keyEvent.state());
            if (result) {
                return true;
            }
        } else {
            ibus_input_context_process_key_event_async(iBusICMap_[id],
                                                       keyEvent.keyValue(),
                                                       keyEvent.keycode() - 8,
                                                       keyEvent.state(),
                                                       -1,
                                                       nullptr,
                                                       processKeyEventDone,
                                                       nullptr);

            result = true;
        }
    }

    return result;
}

void DimIBusProxy::updateInputMethods()
{
    if (!bus_) {
        return;
    }

    auto ibusEngineList = ibus_bus_list_engines(bus_);

    if (!ibusEngineList) {
        qWarning() << "failed get ibus engines list";
        return;
    }

    QList<InputMethodEntry> inputMethods;
    for (; ibusEngineList; ibusEngineList = g_list_next(ibusEngineList)) {
        IBusEngineDesc *engine_desc = IBUS_ENGINE_DESC(ibusEngineList->data);
        g_assert(engine_desc);

        inputMethods.append({ key(),
                              ibus_engine_desc_get_longname(engine_desc),
                              ibus_engine_desc_get_name(engine_desc),
                              ibus_engine_desc_get_description(engine_desc),
                              QString(),
                              ibus_engine_desc_get_icon(engine_desc) });
    }

    inputMethods_.swap(inputMethods);
    g_list_free(ibusEngineList);
}

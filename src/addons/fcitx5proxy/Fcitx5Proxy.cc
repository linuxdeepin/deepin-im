#include "Fcitx5Proxy.h"

#include "DBusProvider.h"
#include "dimcore/Events.h"
#include "dimcore/InputContext.h"
#include "utils/common.h"

#include <QGuiApplication>

using namespace org::deepin::dim;

DIM_ADDON_FACTORY(Fcitx5Proxy);

Fcitx5Proxy::Fcitx5Proxy(Dim *dim)
    : ProxyAddon(dim, "fcitx5proxy")
    , dbusProvider_(new DBusProvider(this))
    , available_(dbusProvider_->available())
{
    connect(dbusProvider_, &DBusProvider::availabilityChanged, this, [this](bool available) {
        if (available_ != available) {
            available_ = available;

            available_ ? updateInputMethods() : inputMethods_.clear();
        }
    });

    updateInputMethods();
}

Fcitx5Proxy::~Fcitx5Proxy()
{
    icMap_.clear();
}

QList<InputMethodEntry> Fcitx5Proxy::getInputMethods()
{
    return inputMethods_;
}

void Fcitx5Proxy::createFcitxInputContext(InputContext *ic)
{
    if (!ic) {
        return;
    }

    FcitxQtStringKeyValueList list;
    FcitxQtStringKeyValue arg;

    arg.setKey("program");
    // TODO: it must be actual app name
    arg.setValue(QStringLiteral("dim"));
    list << arg;

    FcitxQtStringKeyValue arg2;
    arg2.setKey("display");
    arg2.setValue("x11:");
    list << arg2;

    auto result = dbusProvider_->imProxy()->CreateInputContext(list);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result, this);
    QObject::connect(watcher,
                     &QDBusPendingCallWatcher::finished,
                     this,
                     [this, ic](QDBusPendingCallWatcher *watcher) {
                         watcher->deleteLater();
                         QDBusPendingReply<QDBusObjectPath, QByteArray> reply = *watcher;
                         if (reply.isError()) {
                             qDebug()
                                 << "create fcitx input context error:" << reply.error().message();
                             return;
                         }

                         QDBusInterface *icIface =
                             new QDBusInterface(QStringLiteral("org.fcitx.Fcitx5"),
                                                reply.value().path(),
                                                QStringLiteral("org.fcitx.Fcitx.InputContext1"),
                                                QDBusConnection::sessionBus(),
                                                this);
                         if (icIface && icIface->isValid()) {
                             icMap_[ic->id()] = icIface;
                         }
                     });
}

void Fcitx5Proxy::focusIn(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->asyncCall("FocusIn");
    }
}

void Fcitx5Proxy::focusOut(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->asyncCall("FocusOut");
    }
}

void Fcitx5Proxy::destroyed(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->asyncCall("DestroyIC");
    }
}

void Fcitx5Proxy::keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent)
{
    Q_UNUSED(entry);
    auto id = keyEvent.ic()->id();

    if (isICDBusInterfaceValid(id)) {
        auto response = icMap_[id]->call("ProcessKeyEventBatch ",
                                         keyEvent.keyValue(),
                                         keyEvent.keycode(),
                                         keyEvent.state(),
                                         keyEvent.isRelease(),
                                         keyEvent.time());

        if (response.type() == QDBusMessage::ReplyMessage) {
            auto ic = keyEvent.ic();
            // 从返回参数获取返回值
            auto value = response.arguments().takeFirst();
            if (value.canConvert<QList<BatchEvent>>()) {
                QList<BatchEvent> events = value.value<QList<BatchEvent>>();

                for (const auto &event : events) {
                    auto type = std::get<0>(event);
                    auto v = std::get<1>(event);
                    switch (type) {
                    case BATCHED_COMMIT_STRING: {
                        if (v.canConvert<QString>()) {
                            ic->updateCommitString(v.toString());
                        }
                        break;
                    }
                    case BATCHED_PREEDIT: {
                        if (v.canConvert<PreeditKey>()) {
                            ic->updatePreedit(v.value<PreeditKey>());
                        }
                        break;
                    }
                    case BATCHED_FORWARD_KEY: {
                        if (v.canConvert<ForwardKey>()) {
                            ic->forwardKey(v.value<ForwardKey>());
                        }
                        break;
                    }
                    default:
                        qDebug() << "invalid event type " << type;
                        return;
                    }
                }
            }
        }
    }
}

void Fcitx5Proxy::updateInputMethods()
{
    auto call = dbusProvider_->controller()->AvailableInputMethods();
    auto watcher = new QDBusPendingCallWatcher(call, this);
    connect(
        watcher,
        &QDBusPendingCallWatcher::finished,
        this,
        [this](QDBusPendingCallWatcher *watcher) {
            QDBusPendingReply<FcitxQtInputMethodEntryList> ims = *watcher;
            watcher->deleteLater();

            QList<InputMethodEntry> inputMethods;
            for (auto &im : ims.value()) {
                inputMethods.append(
                    { key(), im.uniqueName(), im.name(), im.nativeName(), im.label(), im.icon() });
            }

            inputMethods_.swap(inputMethods);
        });
}

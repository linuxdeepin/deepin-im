// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Fcitx5Proxy.h"

#include "BatchEvent.h"
#include "DBusProvider.h"
#include "dimcore/Dim.h"
#include "dimcore/Events.h"
#include "dimcore/InputContext.h"

#include <QGuiApplication>

using namespace org::deepin::dim;

// This need to keep sync with fcitx5.
enum FcitxCapabilityFlag : uint64_t {
    FcitxCapabilityFlag_Preedit = (1 << 1),
    FcitxCapabilityFlag_Password = (1 << 3),
    FcitxCapabilityFlag_FormattedPreedit = (1 << 4),
    FcitxCapabilityFlag_ClientUnfocusCommit = (1 << 5),
    FcitxCapabilityFlag_SurroundingText = (1 << 6),
    FcitxCapabilityFlag_Email = (1 << 7),
    FcitxCapabilityFlag_Digit = (1 << 8),
    FcitxCapabilityFlag_Uppercase = (1 << 9),
    FcitxCapabilityFlag_Lowercase = (1 << 10),
    FcitxCapabilityFlag_NoAutoUpperCase = (1 << 11),
    FcitxCapabilityFlag_Url = (1 << 12),
    FcitxCapabilityFlag_Dialable = (1 << 13),
    FcitxCapabilityFlag_Number = (1 << 14),
    FcitxCapabilityFlag_NoSpellCheck = (1 << 17),
    FcitxCapabilityFlag_Alpha = (1 << 21),
    FcitxCapabilityFlag_GetIMInfoOnFocus = (1 << 23),
    FcitxCapabilityFlag_RelativeRect = (1 << 24),

    FcitxCapabilityFlag_Multiline = (1ull << 35),
    FcitxCapabilityFlag_Sensitive = (1ull << 36),
    FcitxCapabilityFlag_KeyEventOrderFix = (1ull << 37),
    FcitxCapabilityFlag_ReportKeyRepeat = (1ull << 38),
    FcitxCapabilityFlag_ClientSideInputPanel = (1ull << 39),
    FcitxCapabilityFlag_Disable = (1ull << 40),
};

enum FcitxTextFormatFlag : int {
    FcitxTextFormatFlag_Underline = (1 << 3), /**< underline is a flag */
    FcitxTextFormatFlag_HighLight = (1 << 4), /**< highlight the preedit */
    FcitxTextFormatFlag_DontCommit = (1 << 5),
    FcitxTextFormatFlag_Bold = (1 << 6),
    FcitxTextFormatFlag_Strike = (1 << 7),
    FcitxTextFormatFlag_Italic = (1 << 8),
    FcitxTextFormatFlag_None = 0,
};

DIM_ADDON_FACTORY(Fcitx5Proxy);

Fcitx5Proxy::Fcitx5Proxy(Dim *dim)
    : ProxyAddon(dim, "fcitx5proxy", "org.fcitx.Fcitx5")
    , dbusProvider_(new DBusProvider(this))
    , available_(dbusProvider_->available())
{
    registerBatchEventQtDBusTypes();

    connect(dbusProvider_, &DBusProvider::availabilityChanged, this, [dim, this](bool available) {
        if (available_ != available) {
            available_ = available;

            updateInputMethods();

            if (available_) {
                const auto &inputContexts = dim->getInputContexts();

                for (auto i = inputContexts.begin(); i != inputContexts.end(); ++i) {
                    createFcitxInputContext(i.value());
                }
            }
        }
    });
}

void Fcitx5Proxy::initInputMethods()
{
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

    if (!available_) {
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
    QObject::connect(
        watcher,
        &QDBusPendingCallWatcher::finished,
        this,
        [this, ic](QDBusPendingCallWatcher *watcher) {
            watcher->deleteLater();
            QDBusPendingReply<QDBusObjectPath, QByteArray> reply = *watcher;
            if (reply.isError()) {
                qDebug() << "create fcitx input context error:" << reply.error().message();
                return;
            }

            auto *icIface = new org::fcitx::Fcitx::InputContext1(QStringLiteral("org.fcitx.Fcitx5"),
                                                                 reply.value().path(),
                                                                 QDBusConnection::sessionBus(),
                                                                 this);
            if (icIface && icIface->isValid()) {
                icMap_[ic->id()] = icIface;
            }
        });
}

void Fcitx5Proxy::addCapability(uint32_t id)
{
    quint64 flag = 0;
    flag |= FcitxCapabilityFlag_Preedit;
    flag |= FcitxCapabilityFlag_FormattedPreedit;
    flag |= FcitxCapabilityFlag_ClientUnfocusCommit;
    flag |= FcitxCapabilityFlag_GetIMInfoOnFocus;
    flag |= FcitxCapabilityFlag_KeyEventOrderFix;
    flag |= FcitxCapabilityFlag_ReportKeyRepeat;
    flag |= FcitxCapabilityFlag_SurroundingText;
    flag |= FcitxCapabilityFlag_RelativeRect;
    flag |= FcitxCapabilityFlag_ClientSideInputPanel;

    // Notify fcitx of the effective bits from 0bit to 40bit
    // (FcitxCapabilityFlag_Disable)
    icMap_[id]->SetSupportedCapability(0x1ffffffffffull);

    icMap_[id]->SetCapability(flag);
}

void Fcitx5Proxy::focusIn(uint32_t id)
{
    if (isICDBusInterfaceValid(id)) {
        icMap_[id]->asyncCall("FocusIn");
        addCapability(id);
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

bool Fcitx5Proxy::keyEvent(InputContextKeyEvent &keyEvent)
{
    auto id = keyEvent.ic()->id();

    if (!isICDBusInterfaceValid(id)) {
        return false;
    }

    keyEvent.ic()->setAsyncMode(false);

    auto response = icMap_[id]->ProcessKeyEventBatch(keyEvent.keyValue(),
                                                     keyEvent.keycode(),
                                                     keyEvent.state(),
                                                     keyEvent.isRelease(),
                                                     keyEvent.time());
    response.waitForFinished();
    bool res = response.argumentAt<1>();
    if (!res) {
        return false;
    }

    QList<BatchEvent> events = response.argumentAt<0>();
    auto ic = keyEvent.ic();
    // 从返回参数获取返回值

    for (const auto &event : events) {
        auto type = event.type;
        auto v = event.data;
        switch (type) {
        case BATCHED_COMMIT_STRING: {
            if (v.canConvert<QString>()) {
                ic->updateCommitString(v.toString());
            }
            break;
        }
        case BATCHED_PREEDIT: {
            auto preeditKey = qdbus_cast<PreeditKey>(v.value<QDBusArgument>());

            auto dataList = preeditKey.info;
            auto cursor = preeditKey.cursor;
            QString text;
            for (auto &data : dataList) {
                text.append(data.text);
            }

            ic->updatePreedit(text, cursor, cursor);
            break;
        }
        case BATCHED_FORWARD_KEY: {
            auto forwardKey = qdbus_cast<DBusForwardKey>(v.value<QDBusArgument>());
            ic->forwardKey(forwardKey.keysym, forwardKey.isRelease);
            break;
        }
        default:
            qDebug() << "invalid event type " << type;
            return false;
        }
    }

    return true;
}

void Fcitx5Proxy::updateInputMethods()
{
    if (!available_) {
        inputMethods_.clear();
        return;
    }

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
            Q_EMIT addonInitFinished(this);
        });
}

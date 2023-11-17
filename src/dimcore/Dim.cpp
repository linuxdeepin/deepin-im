// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Dim.h"

#include "Addon.h"
#include "Dconfig.h"
#include "FrontendAddon.h"
#include "InputContext.h"
#include "InputMethodAddon.h"
#include "ProxyAddon.h"
#include "common/common.h"
#include "config.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>

#include <dlfcn.h>

constexpr uint32_t DIM_INPUT_METHOD_SWITCH_KEYBINDING_CODE = SHIFT_MASK | CONTROL_MASK;

#ifdef Dtk6Core_FOUND
const QString DimDConfigAppID = QStringLiteral("org.deepin.dde.dim");
const QString DimDConfigJson = QStringLiteral("org.deepin.dde.dim");
const QString KeyPerWindow = QStringLiteral("Per_Window");
const QString KeyCurrentInputSource = QStringLiteral("Current_Input_Source");
const QString KeyAllInputSources = QStringLiteral("All_Input_Sources");
const QString KeyCurrentUserInputSources = QStringLiteral("Current_User_Input_Sources");
#endif

using namespace org::deepin::dim;

static const QMap<QString, AddonType> AddonsType = {
    { "Frontend", AddonType::Frontend },
    { "InputMethod", AddonType::InputMethod },
};

static bool isExecutableExisted(const QString &name)
{
    return !QStandardPaths::findExecutable(name, QStringList()).isEmpty();
}

Dim::Dim(QObject *parent)
    : QObject(parent)
    , focusedInputContext_(0)
    , activeInputMethodEntries_({ { "keyboard", "us" } }) // todo: load from config file
    , currentActiveIMEntries_({ { "keyboard", "us" } })
#ifdef Dtk6Core_FOUND
    , dimConf_(DconfigSettings::ConfigPtr(DimDConfigAppID, DimDConfigJson))
#endif
{
    launchInputMethodProxyDaemon(Fcitx5DaemonMask | IBusDaemonMask);
    loadAddons();

    // TODO: read current active ims(currentActiveIMEntries_) and current active
    // im(currentActiveIM_) from config file, and to start daemon of ims
#ifdef Dtk6Core_FOUND
    initDConfig();
#endif
}

Dim::~Dim() { }

#ifdef Dtk6Core_FOUND
void Dim::initDConfig()
{
    // 绑定dsg属性
    if (dimConf_) {
        connect(dimConf_, &DConfig::valueChanged, this, [&](const QString &key) {
            if (key == KeyPerWindow) {
                // TODO:
            }
        });
    }
}
#endif

void Dim::loadAddons()
{
    QDir dir(DIM_ADDON_INFO_DIR);
    qInfo() << "addon info dir" << dir.absolutePath();
    auto addonInfoFiles = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Readable);
    for (const auto &addonInfoFile : addonInfoFiles) {
        QString filename = addonInfoFile.fileName();
        if (filename.startsWith('.') || !filename.endsWith(".conf")) {
            continue;
        }

        loadAddon(addonInfoFile.absoluteFilePath());
    }
}

void Dim::loadAddon(const QString &infoFile)
{
    QSettings settings(infoFile, QSettings::Format::IniFormat);
    settings.beginGroup("Addon");
    if (!settings.contains("Name") || !settings.contains("Category")
        || !settings.contains("Library")) {
        qWarning() << "Addon info file" << infoFile << "is invalid";
        return;
    }

    QString name = settings.value("Name").toString();
    QString category = settings.value("Category").toString();
    QString library = settings.value("Library").toString();
    settings.endGroup();

    QDir addonDir(DIM_ADDON_DIR);
    QString libraryFile = library + ".so";
    if (!addonDir.exists(libraryFile)) {
        qWarning() << "Addon library" << libraryFile << "not found";
        return;
    }

    QString libraryPath = addonDir.filePath(libraryFile);
    void *handle = dlopen(libraryPath.toStdString().c_str(), RTLD_LAZY);
    if (handle == nullptr) {
        qWarning() << "Addon library" << libraryFile << "failed to load:" << dlerror();
        return;
    }

    void *createFn = dlsym(handle, "create");
    if (createFn == nullptr) {
        qWarning() << "Addon library" << libraryFile << "does not have a create() function";
        return;
    }

    auto create = reinterpret_cast<addonCreate *>(createFn);
    Addon *addon = create(this);

    switch (AddonsType[category]) {
    case AddonType::Frontend: {
        auto *frontend = qobject_cast<FrontendAddon *>(addon);
        frontends_.insert(frontend);
        break;
    }
    case AddonType::InputMethod: {
        auto *imAddon = qobject_cast<InputMethodAddon *>(addon);
        connect(imAddon, &InputMethodAddon::addonInitFinished, this, &Dim::initInputMethodAddon);
        imAddon->initInputMethods();
        inputMethodAddons_.emplace(imAddon->key(), imAddon);
        break;
    }
    default:
        qWarning() << "Addon" << name << "has an invalid category" << category;
        delete addon;
    }
}

void Dim::initInputMethodAddon(InputMethodAddon *imAddon)
{
    for (const auto &entry : imAddon->getInputMethods()) {
        imEntries_.emplace_back(entry);
    }

    if (!imEntries_.empty()) {
        QTimer::singleShot(0, [this]() {
            Q_EMIT inputMethodEntryChanged();
        });
    }
}

bool Dim::postEvent(Event &event)
{
    switch (event.type()) {
    case EventType::InputContextCreated:
        postInputContextCreated(reinterpret_cast<InputContextEvent &>(event));
        break;
    case EventType::InputContextDestroyed:
        postInputContextDestroyed(reinterpret_cast<InputContextEvent &>(event));
        break;
    case EventType::InputContextFocused:
        postInputContextFocused(reinterpret_cast<InputContextEvent &>(event));
        break;
    case EventType::InputContextUnfocused:
        postInputContextUnfocused(reinterpret_cast<InputContextEvent &>(event));
        break;
    case EventType::InputContextKeyEvent:
        return postInputContextKeyEvent(reinterpret_cast<InputContextKeyEvent &>(event));
        break;
    case EventType::InputContextCursorRectChanged:
        postInputContextCursorRectChanged(
            reinterpret_cast<InputContextCursorRectChangeEvent &>(event));
        break;
    case EventType::InputContextUpdateSurroundingText:
        postInputContextSetSurroundingTextEvent(reinterpret_cast<InputContextEvent &>(event));
        break;
    case EventType::ProxyActiveInputMethodsChanged:
        postProxyActivateInputMethodChanged(reinterpret_cast<ProxyEvent &>(event));
        break;
    }

    return false;
}

void Dim::postInputContextCreated(InputContextEvent &event)
{
    auto *ic = event.ic();

    connect(ic, &InputContext::imSwitch, this, &Dim::switchIM);

    inputContexts_.emplace(ic->id(), ic);

    loopProxyAddon([ic](ProxyAddon *addon) {
        addon->createFcitxInputContext(ic);
    });
}

void Dim::postInputContextDestroyed(InputContextEvent &event)
{
    auto *ic = event.ic();

    inputContexts_.erase(ic->id());

    loopProxyAddon([ic](ProxyAddon *addon) {
        addon->destroyed(ic->id());
    });
}

void Dim::postInputContextFocused(InputContextEvent &event)
{
    auto *ic = event.ic();

    focusedInputContext_ = ic->id();
    emit focusedInputContextChanged(focusedInputContext_);

    loopProxyAddon([ic](ProxyAddon *addon) {
        addon->focusIn(ic->id());
    });
}

void Dim::postInputContextUnfocused(InputContextEvent &event)
{
    auto *ic = event.ic();

    focusedInputContext_ = 0;
    emit focusedInputContextChanged(focusedInputContext_);

    loopProxyAddon([ic](ProxyAddon *addon) {
        addon->focusOut(ic->id());
    });
}

bool Dim::postInputContextKeyEvent(InputContextKeyEvent &event)
{
    auto &inputState = event.ic()->inputState();

    if (event.isRelease() && event.state() == DIM_INPUT_METHOD_SWITCH_KEYBINDING_CODE) {
        QTimer::singleShot(0, [ic = event.ic()]() {
            ic->inputState().switchIMAddon();
        });
    }

    auto addon = getInputMethodAddon(inputState);

    auto entryIter = std::find_if(
        imEntries_.cbegin(),
        imEntries_.cend(),
        [&currentIMEntry = inputState.currentIMEntry()](const InputMethodEntry &entry) {
            return entry.addonKey() == currentIMEntry.first
                && entry.uniqueName() == currentIMEntry.second;
        });
    if (addon) {
        return addon->keyEvent(*entryIter, event);
    }

    return false;
}

void Dim::postInputContextCursorRectChanged(InputContextCursorRectChangeEvent &event)
{
    auto addon = qobject_cast<ProxyAddon *>(getInputMethodAddon(event.ic()->inputState()));
    if (addon) {
        addon->cursorRectangleChangeEvent(event);
    }
}

void Dim::postInputContextSetSurroundingTextEvent(InputContextEvent &event)
{
    auto addon = getInputMethodAddon(event.ic()->inputState());
    if (addon) {
        addon->updateSurroundingText(event);
    }
}

void Dim::postProxyActivateInputMethodChanged(ProxyEvent &event)
{
    auto *proxyAddon = event.proxyAddon();
    std::string addonKey = proxyAddon->key();

    auto &list = proxyAddon->activeInputMethods();
    for (const auto &entry : list) {
        addActiveInputMethodEntry(addonKey, entry);
    }
}

void Dim::addActiveInputMethodEntry(const std::string &addon, const std::string &entry)
{
    auto [_, res] = activeInputMethodEntries_.emplace(std::make_pair(addon, entry));
    if (!res) {
        return;
    }

    // todo: save config
}

InputMethodAddon *Dim::getInputMethodAddon(const InputState &inputState)
{
    const std::string &addonKey = inputState.currentIMEntry().first;
    auto j = imAddons().find(addonKey);
    assert(j != imAddons().end());

    return j->second;
}

void Dim::loopProxyAddon(const std::function<void(ProxyAddon *addon)> callback)
{
    for (const auto &[_, v] : inputMethodAddons_) {
        auto addon = qobject_cast<ProxyAddon *>(v);
        if (addon) {
            callback(addon);
        }
    }
}

void Dim::switchIM(const std::pair<std::string, std::string> &imIndex)
{
    auto addon = qobject_cast<ProxyAddon *>(imAddons().at(imIndex.first));

    // TODO: update currentActiveIM_ to config file

    if (addon) {
        addon->setCurrentIM(imIndex.second);

        currentActiveIM = imIndex;
    }
}

void Dim::launchFcitx5Daemon()
{
    if (!isExecutableExisted(QStringLiteral("fcitx5"))) {
        qDebug() << "can not find fcitx5 executable, maybe it should be installed";
        return;
    }

    QProcess fcitx5Proc;

    fcitx5Proc.startDetached(
        QStringLiteral("fcitx5"),
        QStringList{ QStringLiteral("--disable"),
                     QStringLiteral("fcitx4frontend,ibusfrontend,xim,waylandim,notificationitem"),
                     QStringLiteral("-r"),
                     QStringLiteral("-d") });
    auto ret = fcitx5Proc.waitForStarted(500);
    if (!ret) {
        qWarning() << "launch fcitx5 failed";
    }
}

void Dim::launchIbusDaemon()
{
    if (!isExecutableExisted(QStringLiteral("ibus-daemon"))) {
        qDebug() << "can not find ibus daemon executable, maybe it should be installed";
        return;
    }

    QProcess ibusDaemonProc;

    ibusDaemonProc.startDetached(QStringLiteral("ibus-daemon"),
                                 QStringList{ QStringLiteral("-r"), QStringLiteral("-d") });
    auto ret = ibusDaemonProc.waitForStarted(500);
    if (!ret) {
        qWarning() << "launch ibus daemon failed";
    }
}

void Dim::launchInputMethodProxyDaemon(uint32_t modifier)
{
    if (modifier & Fcitx5DaemonMask) {
        launchFcitx5Daemon();
    }

    if (modifier & IBusDaemonMask) {
        launchIbusDaemon();
    }
}

bool Dim::requestSwitchIM(const std::string &addon, const std::string &name)
{
    auto iter = std::find_if(activeInputMethodEntries().cbegin(),
                             activeInputMethodEntries().cend(),
                             [&addon, &name](const auto &pair) {
                                 return (pair.first == addon) && pair.second == name;
                             });
    if (iter == activeInputMethodEntries().cend()) {
        qDebug() << "invalid input method " << QString::fromStdString(name);
        return false;
    }

    auto currentFocusedIc = getInputContext(focusedInputContext());
    if (currentFocusedIc) {
        currentFocusedIc->inputState().requestSwitchIM(addon, name);
        return true;
    }

    return false;
}

/*
 * Description: add input method
 * addon: input method framework(ibus or fcitx5)
 * name: input method name
 */
void Dim::addInputMethod(const std::string &addon, const std::string &name)
{
    auto iter = std::find_if(activeInputMethodEntries().cbegin(),
                             activeInputMethodEntries().cend(),
                             [&addon, &name](const auto &pair) {
                                 return (pair.first == addon) && pair.second == name;
                             });
    if (iter == activeInputMethodEntries().cend()) {
        qDebug() << "invalid input method " << QString::fromStdString(name);
        return;
    }

    currentActiveIMEntries_.emplace(*iter);
    // TODO: save to config
}

/*
 * Description: remove input method
 * addon: input method framework(ibus or fcitx)
 * name: input method name
 */
void Dim::removeInputMethod(const std::string &addon, const std::string &name)
{
    auto iter = std::find_if(getCurrentInputMethods().cbegin(),
                             getCurrentInputMethods().cend(),
                             [&addon, &name](const auto &pair) {
                                 return (pair.first == addon) && pair.second == name;
                             });
    if (iter == getCurrentInputMethods().cend()) {
        qDebug() << "invalid input method " << QString::fromStdString(name);
        return;
    }

    currentActiveIMEntries_.erase(iter);
    // TODO: remove from config
}

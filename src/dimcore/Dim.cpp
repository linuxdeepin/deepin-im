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
#include <QSettings>
#include <QTimer>

#include <dlfcn.h>

constexpr uint32_t DIM_INPUT_METHOD_SWITCH_KEYBINDING_CODE = SHIFT_MASK | CONTROL_MASK;

#ifdef Dtk6Core_FOUND
const QString DimDConfigAppID = QStringLiteral("org.deepin.dde.dim");
const QString DimDConfigJson = QStringLiteral("org.deepin.dde.dim");
const QString ShareInputState = QStringLiteral("ShareInputState");
const QString KeyCurrentInputSource = QStringLiteral("CurrentInputSource");
const QString KeyCurrentUserInputSources = QStringLiteral("CurrentUserInputSources");
#endif

using namespace org::deepin::dim;

static const QMap<QString, AddonType> AddonsType = {
    { "Frontend", AddonType::Frontend },
    { "InputMethod", AddonType::InputMethod },
};

namespace org::deepin::dim {
struct AddonDesc
{
    QString addon;
    QString name;
    QString category;
    QString library;
    QStringList dependencies;
};
} // namespace org::deepin::dim

Dim::Dim(QObject *parent)
    : QObject(parent)
    , focusedInputContext_(0)
#ifdef Dtk6Core_FOUND
    , dimConf_(DconfigSettings::ConfigPtr(DimDConfigAppID, DimDConfigJson))
#endif
{
    loadAddons();

#ifdef Dtk6Core_FOUND
    initDConfig();
#else
    currentActiveIM_ = std::make_pair("keyboard", "us");
    activeInputMethodEntries_ = { { "keyboard", "us" } };
#endif
}

Dim::~Dim() { }

#ifdef Dtk6Core_FOUND
void Dim::initDConfig()
{
    // 绑定dsg属性
    if (dimConf_) {
        connect(dimConf_, &DConfig::valueChanged, this, [&](const QString &key) {
            if (key == ShareInputState) {
                // TODO:
            }
        });
    }

    QVariant dconfIMKey =
        DconfigSettings::ConfigValue(DimDConfigAppID, DimDConfigJson, KeyCurrentInputSource, "");
    if (dconfIMKey.isValid()) {
        currentActiveIM_ = keyToIndex(dconfIMKey.toString());
    }

    QVariant dconfIMKeys = DconfigSettings::ConfigValue(DimDConfigAppID,
                                                        DimDConfigJson,
                                                        KeyCurrentUserInputSources,
                                                        "");
    if (dconfIMKeys.isValid()) {
        for (const auto &imKey : dconfIMKeys.toStringList()) {
            activeInputMethodEntries_.emplace(keyToIndex(imKey));
        }
    }
}
#endif

void Dim::loadAddons()
{
    QDir dir(DIM_ADDON_INFO_DIR);
    qInfo() << "addon info dir" << dir.absolutePath();
    auto addonInfoFiles = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Readable);

    std::vector<AddonDesc> list;
    for (const auto &addonInfoFile : addonInfoFiles) {
        QString filename = addonInfoFile.fileName();
        if (filename.startsWith('.') || !filename.endsWith(".conf")) {
            continue;
        }

        QSettings settings(addonInfoFile.absoluteFilePath(), QSettings::Format::IniFormat);
        settings.beginGroup("Addon");
        if (!settings.contains("Name") || !settings.contains("Category")
            || !settings.contains("Library")) {
            qDebug() << "Addon info file" << addonInfoFile << "is invalid";
            continue;
        }

        list.emplace_back(AddonDesc{
            addonInfoFile.baseName(),
            settings.value("Name").toString(),
            settings.value("Category").toString(),
            settings.value("Library").toString(),
            settings.value("Dependencies").toStringList(),
        });

        settings.endGroup();
    }

    std::sort(list.begin(), list.end(), [](const AddonDesc &l, const AddonDesc &r) {
        return !l.dependencies.contains(r.addon);
    });

    for (auto &a : list) {
        loadAddon(a);
    }
}

void Dim::loadAddon(const AddonDesc &info)
{
    QDir addonDir(DIM_ADDON_DIR);
    QString libraryFile = info.library + ".so";
    if (!addonDir.exists(libraryFile)) {
        qWarning() << "Addon library" << libraryFile << "not found";
        return;
    }

    QString libraryPath = addonDir.filePath(libraryFile);
    qDebug() << "loading addon:" << libraryPath;
    void *handle = dlopen(libraryPath.toStdString().c_str(), RTLD_NOW | RTLD_GLOBAL);
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

    switch (AddonsType[info.category]) {
    case AddonType::Frontend: {
        addons_.emplace(addon->key(), addon);
        break;
    }
    case AddonType::InputMethod: {
        auto *imAddon = qobject_cast<InputMethodAddon *>(addon);
        connect(imAddon, &InputMethodAddon::addonInitFinished, this, &Dim::initInputMethodAddon);
        imAddon->initInputMethods();
        addons_.emplace(addon->key(), addon);
        break;
    }
    default:
        qWarning() << "Addon" << info.name << "has an invalid category" << info.category;
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

#ifdef Dtk6Core_FOUND
    updateDconfInputMethodEntries();
#endif
}

InputMethodAddon *Dim::getInputMethodAddon(const InputState &inputState)
{
    const std::string &addonKey = inputState.currentIMEntry().first;
    auto j = addons().find(addonKey);
    assert(j != addons().end());

    auto *imAddon = qobject_cast<InputMethodAddon *>(j->second);
    assert(imAddon);

    return imAddon;
}

void Dim::loopProxyAddon(const std::function<void(ProxyAddon *addon)> callback)
{
    for (const auto &[_, v] : addons_) {
        auto addon = qobject_cast<ProxyAddon *>(v);
        if (addon) {
            callback(addon);
        }
    }
}

void Dim::switchIM(const std::pair<std::string, std::string> &imIndex)
{
    auto addon = qobject_cast<ProxyAddon *>(addons().at(imIndex.first));

    if (addon) {
        addon->setCurrentIM(imIndex.second);

        currentActiveIM_ = imIndex;

#ifdef Dtk6Core_FOUND
        DconfigSettings::ConfigSaveValue(DimDConfigAppID,
                                         DimDConfigJson,
                                         KeyCurrentInputSource,
                                         QVariant::fromValue(indexToKey(currentActiveIM_)));
#endif
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
 * addon: input method framework(ibus or fcitx5 or keyboard)
 * name: input method name
 */
void Dim::addInputMethod(const std::string &addon, const std::string &name)
{
    auto iter = std::find_if(imEntries().cbegin(),
                             imEntries().cend(),
                             [&addon, &name](const InputMethodEntry &entry) {
                                 return (entry.addonKey() == addon) && entry.uniqueName() == name;
                             });
    if (iter == imEntries().cend()) {
        qDebug() << "invalid input method " << QString::fromStdString(name);
        return;
    }

    activeInputMethodEntries_.emplace(std::make_pair(iter->addonKey(), iter->uniqueName()));
#ifdef Dtk6Core_FOUND
    updateDconfInputMethodEntries();
#endif
}

/*
 * Description: remove input method
 * addon: input method framework(ibus or fcitx)
 * name: input method name
 */
void Dim::removeInputMethod(const std::string &addon, const std::string &name)
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

    activeInputMethodEntries_.erase(iter);
#ifdef Dtk6Core_FOUND
    updateDconfInputMethodEntries();
#endif
}

QString Dim::indexToKey(const std::pair<std::string, std::string> &imIndex) const
{
    return QString("%1:%2")
        .arg(QString::fromStdString(imIndex.first))
        .arg(QString::fromStdString(imIndex.second));
}

const std::pair<std::string, std::string> Dim::keyToIndex(const QString &imKey) const
{
    QStringList keys = imKey.split(":");

    // TODO: need to judge validity of imKey
    Q_ASSERT(keys.size() == 2);

    return std::make_pair(keys[0].toStdString(), keys[1].toStdString());
}

#ifdef Dtk6Core_FOUND
void Dim::updateDconfInputMethodEntries() const
{
    QVariant dconfIMKeys = DconfigSettings::ConfigValue(DimDConfigAppID,
                                                        DimDConfigJson,
                                                        KeyCurrentUserInputSources,
                                                        "");
    if (!dconfIMKeys.isValid()) {
        return;
    }
    auto imKeys = dconfIMKeys.toStringList();

    for (const auto &entry : activeInputMethodEntries()) {
        const auto &key = indexToKey(entry);
        if (!imKeys.contains(key)) {
            imKeys.append(key);
        }
    }

    DconfigSettings::ConfigSaveValue(DimDConfigAppID,
                                     DimDConfigJson,
                                     KeyCurrentUserInputSources,
                                     QVariant::fromValue(imKeys));
}
#endif

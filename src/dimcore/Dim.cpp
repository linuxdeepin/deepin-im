// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Dim.h"

#include "Addon.h"
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

using namespace org::deepin::dim;

static const QMap<QString, AddonType> AddonsType = {
    { "Frontend", AddonType::Frontend },
    { "InputMethod", AddonType::InputMethod },
};

Dim::Dim(QObject *parent)
    : QObject(parent)
    , focusedInputContext_(0)
    , activeInputMethodEntries_({ { "keyboard", "us" } }) // todo: load from config file
{
    loadAddons();
}

Dim::~Dim() { }

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
        postInputContextCreated(reinterpret_cast<InputContextEvent &>(event));
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
    const auto &[addonKey, entryUniqueName] = inputState.currentIMEntry();

    auto entryIter = std::find_if(imEntries_.cbegin(),
                                  imEntries_.cend(),
                                  [&addonKey, &entryUniqueName](const InputMethodEntry &entry) {
                                      return entry.addonKey() == addonKey
                                          && entry.uniqueName() == entryUniqueName;
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

    if (addon) {
        addon->setCurrentIM(imIndex.second);
    }
}

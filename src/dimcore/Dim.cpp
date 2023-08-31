// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Dim.h"

#include "Addon.h"
#include "FrontendAddon.h"
#include "InputContext.h"
#include "InputMethodAddon.h"
#include "ProxyAddon.h"
#include "config.h"
#include "common/common.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <dlfcn.h>

#define DIM_INPUT_METHOD_SWITCH_KEYBINDING_CODE (SHIFT_MASK + CONTROL_MASK)

using namespace org::deepin::dim;

static const QMap<QString, AddonType> AddonsType = {
    { "Frontend", AddonType::Frontend },
    { "InputMethod", AddonType::InputMethod },
};

Dim::Dim(QObject *parent)
    : QObject(parent)
    , focusedIC_(0)
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
        connect(imAddon, &InputMethodAddon::addonInitFinished, this , &Dim::initInputMethodAddon);
        imAddon->initInputMethods();
        inputMethodAddons_.insert(imAddon->key(), imAddon);
        break;
    }
    default:
        qWarning() << "Addon" << name << "has an invalid category" << category;
        delete addon;
    }
}

void Dim::initInputMethodAddon(InputMethodAddon *imAddon)
{
    for (auto &i : imAddon->getInputMethods()) {
        ims_.insert(i.uniqueName(), std::move(i));
    }
}

bool Dim::postEvent(Event &event)
{
    switch (event.type()) {
    case EventType::InputContextCreated:
        postInputContextCreated(event);
        break;
    case EventType::InputContextDestroyed:
        postInputContextCreated(event);
        break;
    case EventType::InputContextFocused:
        postInputContextFocused(event);
        break;
    case EventType::InputContextUnfocused:
        postInputContextUnfocused(event);
        break;
    case EventType::InputContextKeyEvent:
        return postInputContextKeyEvent(reinterpret_cast<InputContextKeyEvent &>(event));
        break;
    case EventType::InputContextCursorRectChanged:
        // TODO:
        break;
    }

    return false;
}

QList<QString> Dim::imAddons() const {
    return inputMethodAddons_.keys();
}

void Dim::postInputContextCreated(Event &event)
{
    auto *ic = event.ic();
    inputContexts_.insert(ic->id(), ic);

    for (auto it = inputMethodAddons_.begin(); it != inputMethodAddons_.end(); ++it) {
        ProxyAddon *addon = qobject_cast<ProxyAddon *>(it.value());
        if (addon) {
            addon->createFcitxInputContext(ic);
        }
    }
}

void Dim::postInputContextDestroyed([[maybe_unused]] Event &event)
{
    inputContexts_.remove(event.ic()->id());
    for (auto it = inputMethodAddons_.begin(); it != inputMethodAddons_.end(); ++it) {
        ProxyAddon *addon = qobject_cast<ProxyAddon *>(it.value());
        if (addon) {
            addon->destroyed(event.ic()->id());
        }
    }
}

ProxyAddon *Dim::getCurrentImAddon()
{
    ProxyAddon *addon = qobject_cast<ProxyAddon *>(currentImAddon);

    return addon != nullptr ? addon : nullptr;
}

void Dim::postInputContextFocused(Event &event)
{
    focusedIC_ = event.ic()->id();

    for (auto it = inputMethodAddons_.begin(); it != inputMethodAddons_.end(); ++it) {
        ProxyAddon *addon = qobject_cast<ProxyAddon *>(it.value());
        if (addon) {
            addon->focusIn(focusedIC_);
        }
    }
}

void Dim::postInputContextUnfocused([[maybe_unused]] Event &event)
{
    focusedIC_ = 0;

    for (auto it = inputMethodAddons_.begin(); it != inputMethodAddons_.end(); ++it) {
        ProxyAddon *addon = qobject_cast<ProxyAddon *>(it.value());
        if (addon) {
            addon->focusOut(event.ic()->id());
        }
    }
}

bool Dim::postInputContextKeyEvent(InputContextKeyEvent &event)
{
    if (event.isRelease() && event.state() == DIM_INPUT_METHOD_SWITCH_KEYBINDING_CODE) {
        Q_EMIT imChanged();
    }

    const auto &inputState = event.ic()->inputState();

    const QString &addonKey = inputState.currentIMAddon();

    auto j = inputMethodAddons_.find(addonKey);
    if (j == inputMethodAddons_.end()) {
        // TODO:
        return false;
    }
    auto *addon = j.value();
    currentImAddon = addon;

    return addon->keyEvent(event);
}

#include "Dim.h"

#include "Addon.h"
#include "FrontendAddon.h"
#include "InputContext.h"
#include "InputMethodAddon.h"
#include "ProxyAddon.h"
#include "config.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <dlfcn.h>

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
        initInputMethodAddon(imAddon);
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
        postKeyEvent(reinterpret_cast<KeyEvent &>(event));
        break;
    case EventType::InputContextCursorRectChanged:
        // TODO:
        break;
    }

    return false;
}

const QMap<QString, InputMethodEntry> &Dim::ims() const
{
    return ims_;
}

const QList<QString> &Dim::enabledIMs() const
{
    return enabledIMs_;
}

void Dim::postInputContextCreated(Event &event)
{
    auto *ic = event.ic();
    inputContexts_.insert(ic->id(), ic);

    for (auto it = inputMethodAddons_.begin(); it != inputMethodAddons_.end(); ++it) {
        // TODO: it must be replaced by actual app name
        ProxyAddon *addon = qobject_cast<ProxyAddon *>(it.value());
        if (addon) {
            addon->createInputContext(ic->id(), QString());
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

void Dim::postKeyEvent(KeyEvent &event)
{
    // TODO: check shortcuts (switch im etc.)

    const auto &inputState = event.ic()->inputState();

    const QString &currentIMKey = inputState.currentIM();
    auto i = ims_.find(currentIMKey);
    if (i == ims_.end()) {
        // TODO:
        return;
    }
    const auto &im = i.value();

    const auto &addonKey = im.addon();
    auto j = inputMethodAddons_.find(addonKey);
    if (j == inputMethodAddons_.end()) {
        // TODO:
        return;
    }
    auto *addon = j.value();

    addon->keyEvent(im, event);
}

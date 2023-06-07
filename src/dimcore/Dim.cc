#include "Dim.h"

#include "Addon.h"
#include "FrontendAddon.h"
#include "InputContext.h"
#include "InputMethodAddon.h"
#include "config.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <dlfcn.h>

using namespace org::deepin::dim;

Dim::Dim(QObject *parent)
    : QObject(parent)
    , focusedIC_(0)
{
    loadAddons();
}

Dim::~Dim() { }

void Dim::inputContextCreated(InputContext *ic)
{
    inputContexts_.insert(ic->id(), ic);

    auto it = inputMethodAddons_.find(QStringLiteral("fcitx5proxy"));
    if (it == inputMethodAddons_.end()) {
        qDebug() << "failed to find fcitx5proxy";
        return;
    }

    // TODO: it must be replaced by actual app name
    Q_EMIT it.value()->createInputContext(QString());

    connect(ic, &InputContext::destroyed, this, [this, id = ic->id(), it]() {
        inputContexts_.remove(id);
        Q_EMIT it.value()->destroyed(QString());
    });
    connect(ic, &InputContext::focused, this, [this, id = ic->id(), it]() {
        focusedIC_ = id;
        Q_EMIT it.value()->focusIn(QString());
    });
    connect(ic, &InputContext::unFocused, this, [this, it]() {
        focusedIC_ = 0;
        Q_EMIT it.value()->focusOut(QString());
    });
}

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

    if (category == "InputMethod") {
        auto *imAddon = qobject_cast<InputMethodAddon *>(addon);
        initInputMethodAddon(imAddon);
        inputMethodAddons_.insert(imAddon->key(), imAddon);
    } else if (category == "Frontend") {
        auto *frontend = qobject_cast<FrontendAddon *>(addon);
        frontends_.insert(frontend);
    } else {
        qWarning() << "Addon" << name << "has an invalid category" << category;
        delete addon;
        return;
    }
}

void Dim::initInputMethodAddon(InputMethodAddon *addon)
{
    // ims_.append(addon->getInputMethods());
    for (auto &i : addon->getInputMethods()) {
        ims_.insert(i.uniqueName(), std::move(i));
    }
}

bool Dim::postEvent(Event &event)
{
    switch (event.type()) {
    case EventType::InputContextCreated:
        // TODO:
        break;
    case EventType::InputContextDestroyed:
        // TODO:
        break;
    case EventType::InputContextFocused:
        // TODO:
        break;
    case EventType::InputContextUnfocused:
        // TODO:
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

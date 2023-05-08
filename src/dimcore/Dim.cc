#include "Dim.h"

#include <dlfcn.h>

#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QPluginLoader>

#include "InputContext.h"
#include "config.h"
#include "Addon.h"

Dim::Dim(QObject *parent)
    : QObject(parent) {
    loadAddons();
}

Dim::~Dim() {
}

uint32_t Dim::newInputContext() {
    auto *ctx = new InputContext(this);
    inputContexts_.insert(ctx->id(), ctx);

    connect(ctx, &InputContext::destroyed, this, [this, ctx]() { inputContexts_.remove(ctx->id()); });

    return ctx->id();
}

void Dim::loadAddons() {
    QDir dir(DIM_ADDON_INFO_DIR);
    auto addonInfoFiles = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Readable);
    for (const auto &addonInfoFile : addonInfoFiles) {
        QString filename = addonInfoFile.fileName();
        if (filename.startsWith('.') || !filename.endsWith(".conf")) {
            continue;
        }

        loadAddon(addonInfoFile.absoluteFilePath());
    }
}

void Dim::loadAddon(const QString &infoFile) {
    QSettings settings(infoFile, QSettings::Format::IniFormat);
    settings.beginGroup("Addons");
    if (!settings.contains("Name") || !settings.contains("Type") || !settings.contains("Category") ||
        !settings.contains("Library")) {
        qWarning() << "Addon info file" << infoFile << "is invalid";
        return;
    }

    QString name = settings.value("Name").toString();
    QString type = settings.value("Type").toString();
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
    addons_.insert(addon);
}

#include "DIMPlatformInputContextPlugin.h"

#include <QDebug>

#include "DIMPlatformInputContext.h"

DIMPlatformInputContextPlugin::DIMPlatformInputContextPlugin(QObject *parent)
    : QPlatformInputContextPlugin(parent) {
}

QPlatformInputContext *DIMPlatformInputContextPlugin ::create(
    const QString &key,
    [[maybe_unused]] const QStringList &paramList) {
    if (key.compare("dim", Qt::CaseInsensitive) == 0) {
        return new DIMPlatformInputContext();
    }
    return nullptr;
}

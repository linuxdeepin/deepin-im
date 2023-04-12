#include "DIMPlatformInputContextPlugin.h"

#include <QDebug>

#include "DIMPlatformInputContext.h"

DIMPlatformInputContextPlugin::DIMPlatformInputContextPlugin(QObject *parent)
    : QPlatformInputContextPlugin(parent) {
}

QPlatformInputContext *DIMPlatformInputContextPlugin ::create(
    const QString &key,
    [[maybe_unused]] const QStringList &paramList) {
    qWarning() << "======================================dddddddddddddddddd";
    if (key.compare("dim", Qt::CaseInsensitive) == 0) {
        qInfo() << "ccccccccccccc";
        return new DIMPlatformInputContext();
    }
    return nullptr;
}

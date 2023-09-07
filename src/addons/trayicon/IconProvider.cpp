#include "IconProvider.h"

static const QSize DEFAULT_SIZE{ 48, 48 };

IconProvider::IconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

IconProvider::~IconProvider() = default;

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    auto s = requestedSize.width() > 0 && requestedSize.height() > 0 ? requestedSize : DEFAULT_SIZE;
    auto pixmap = QIcon::fromTheme(id).pixmap(s);
    *size = s;
    qDebug() << id << s << requestedSize << *size << pixmap;

    return pixmap;
}

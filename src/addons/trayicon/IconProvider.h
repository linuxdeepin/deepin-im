#ifndef TRAYICON_ICONPROVIDER_H
#define TRAYICON_ICONPROVIDER_H

#include <QIcon>
#include <QQuickImageProvider>

class IconProvider : public QQuickImageProvider
{
public:
    IconProvider();
    ~IconProvider() override;

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // !TRAYICON_ICONPROVIDER_H

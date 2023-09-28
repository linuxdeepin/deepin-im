// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TRAYICON_ICONPROVIDER_H
#define TRAYICON_ICONPROVIDER_H

#include <QIcon>
#include <QQuickImageProvider>

class IconProvider : public QQuickImageProvider
{
public:
    explicit IconProvider();
    ~IconProvider() override;

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // !TRAYICON_ICONPROVIDER_H

// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Compositor.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Compositor compositor;
    compositor.setSocketName(qgetenv("DIM_WAYLAND_DISPLAY"));
    compositor.create();

    return app.exec();
}

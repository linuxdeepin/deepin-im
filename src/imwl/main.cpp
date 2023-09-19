// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Compositor.h"

#include <DLog>

#include <QCoreApplication>

using Dtk::Core::DLogManager;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    DLogManager::registerJournalAppender();
    DLogManager::registerConsoleAppender();

    Compositor compositor;
    compositor.addSocket("imfakewl");
    compositor.create();

    return app.exec();
}

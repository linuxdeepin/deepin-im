// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Server.h"

#include <QCoreApplication>

#ifdef Dtk6Core_FOUND
#  include <DLog>

using Dtk::Core::DLogManager;
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

#ifdef Dtk6Core_FOUND
    DLogManager::registerJournalAppender();
    DLogManager::registerConsoleAppender();
#endif

    Server server;
    server.addSocket("imfakewl");
    server.create();

    return app.exec();
}

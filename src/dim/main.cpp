// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dimcore/Dim.h"

#include <QGuiApplication>

#ifdef Dtk6Core_FOUND
#  include <DLog>

using Dtk::Core::DLogManager;
#endif

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

#ifdef Dtk6Core_FOUND
    DLogManager::registerJournalAppender();
    DLogManager::registerConsoleAppender();
#endif

    org::deepin::dim::Dim dim;

    return a.exec();
}

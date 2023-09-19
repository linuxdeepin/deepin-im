// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dimcore/Dim.h"

#include <DLog>

#include <QGuiApplication>

using Dtk::Core::DLogManager;

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    DLogManager::registerJournalAppender();
    DLogManager::registerConsoleAppender();

    org::deepin::dim::Dim dim;

    return a.exec();
}

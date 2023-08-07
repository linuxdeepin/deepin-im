// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>

#include "dimcore/Dim.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    org::deepin::dim::Dim dim;

    return a.exec();
}

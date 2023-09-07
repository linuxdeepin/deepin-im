// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>

#include "dimcore/Dim.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    org::deepin::dim::Dim dim;

    return a.exec();
}

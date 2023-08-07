// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"

#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    auto *lineEdit = new QLineEdit(this);
    lineEdit->setFixedWidth(300);

    setFixedWidth(400);
}

MainWindow::~MainWindow() {
}

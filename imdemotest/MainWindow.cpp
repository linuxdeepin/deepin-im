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

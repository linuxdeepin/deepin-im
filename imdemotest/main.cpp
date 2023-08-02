#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    qputenv("QT_IM_MODULE", QByteArray("dim"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

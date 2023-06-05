#include <QCoreApplication>

#include "dimcore/Dim.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    org::deepin::dim::Dim dim;

    return a.exec();
}

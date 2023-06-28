#include "WLFrontend.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication app(argc, argv);

    org::deepin::dim::WLFrontend frontend;

    return app.exec();
}

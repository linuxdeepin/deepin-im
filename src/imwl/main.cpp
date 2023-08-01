#include "Compositor.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Compositor compositor;
    compositor.setSocketName("imfakewl");
    compositor.create();

    return app.exec();
}

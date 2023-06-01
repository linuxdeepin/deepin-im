#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <QDBusObjectPath>

#include <dimcore/FrontendAddon.h>

class InputmethodAdaptor;
class InputContext1;

class DBusFrontend : public FrontendAddon {
    Q_OBJECT

public:
    explicit DBusFrontend(Dim *dim);
    ~DBusFrontend() override;

public slots:
    QDBusObjectPath CreateInputContext();

private:
    InputmethodAdaptor *adaptor_;
};

#endif // !DBUSFRONTEND_H

#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <QDBusObjectPath>

#include <dimcore/FrontendAddon.h>

class PortalAdaptor;

class DBusFrontend : public FrontendAddon {
public:
    explicit DBusFrontend(Dim *dim);
    ~DBusFrontend() override;

public slots:
    QDBusObjectPath CreateInputContext();

private:
    PortalAdaptor *adaptor_;
};

#endif // !DBUSFRONTEND_H

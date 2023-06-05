#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

class InputmethodAdaptor;

namespace org {
namespace deepin {
namespace dim {

class InputContext1;

class DBusFrontend : public FrontendAddon
{
    Q_OBJECT

public:
    explicit DBusFrontend(Dim *dim);
    ~DBusFrontend() override;

public slots:
    QDBusObjectPath CreateInputContext();

private:
    InputmethodAdaptor *adaptor_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

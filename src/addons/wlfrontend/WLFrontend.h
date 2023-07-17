#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include "WaylandInputContextV2.h"

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

#include <memory>

namespace wl {
class Connection;
}

namespace org {
namespace deepin {
namespace dim {

class InputContext1;

class WLFrontend : public FrontendAddon
{
    Q_OBJECT

public:
    explicit WLFrontend(Dim *dim);
    ~WLFrontend();

private:
    wl::Connection *wl_;

    std::vector<std::shared_ptr<WaylandInputContextV2>> ims_;

    void init();
    void reloadSeats();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

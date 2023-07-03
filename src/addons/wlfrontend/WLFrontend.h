#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

class InputmethodAdaptor;
struct wl_registry;

namespace org {
namespace deepin {
namespace dim {

class WaylandConnection;
class InputContext1;

class WLFrontend : public QObject
{
    Q_OBJECT

public:
    explicit WLFrontend();
    ~WLFrontend();

private:
    WaylandConnection *wl_;

    void registryGlobal(struct wl_registry *registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

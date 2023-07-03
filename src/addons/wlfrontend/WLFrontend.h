#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

class InputmethodAdaptor;
struct wl_registry;
struct zwp_input_method_v1;
struct zwp_input_method_context_v1;

namespace org {
namespace deepin {
namespace dim {

class WaylandConnection;
class InputContext1;

class WLFrontend : public QObject
{
    Q_OBJECT
    friend class zwp_input_method_v1_listener;

public:
    explicit WLFrontend();
    ~WLFrontend();

    void inputMethodActivate(struct zwp_input_method_v1 *zwp_input_method_v1,
                             struct zwp_input_method_context_v1 *id);
    void inputMethodDeactivate(struct zwp_input_method_v1 *zwp_input_method_v1,
                               struct zwp_input_method_context_v1 *id);

private:
    WaylandConnection *wl_;
    struct zwp_input_method_v1 *input_method_v1_;
    struct zwp_input_method_context_v1 *input_method_context_v1_;

    void registryGlobal(struct wl_registry *registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

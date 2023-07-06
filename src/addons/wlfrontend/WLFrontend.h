#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

class WaylandConnection;
struct wl_registry;
struct zwp_input_method_v2;
struct zwp_input_method_context_v2;

namespace org {
namespace deepin {
namespace dim {

class InputContext1;

class WLFrontend : public QObject
{
    Q_OBJECT
    friend class zwp_input_method_v1_listener;

public:
    explicit WLFrontend();
    ~WLFrontend();

    void registryGlobal(struct wl_registry *registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version);
    void inputMethodActivate(struct zwp_input_method_v2 *zwp_input_method_v2);
    void inputMethodDeactivate(struct zwp_input_method_v2 *zwp_input_method_v2);
    void inputMethodSurroundingText(struct zwp_input_method_v2 *zwp_input_method_v2,
                                    const char *text,
                                    uint32_t cursor,
                                    uint32_t anchor);
    void inputMethodTextChangeCause(struct zwp_input_method_v2 *zwp_input_method_v2,
                                    uint32_t cause);
    void inputMethodContentType(struct zwp_input_method_v2 *zwp_input_method_v2,
                                uint32_t hint,
                                uint32_t purpose);
    void inputMethodDone(struct zwp_input_method_v2 *zwp_input_method_v2);
    void inputMethodUnavailable(struct zwp_input_method_v2 *zwp_input_method_v2);

private:
    WaylandConnection *wl_;
    struct zwp_input_method_manager_v2 *input_method_manager_v2_;
    struct zwp_input_method_v2 *input_method_v2_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

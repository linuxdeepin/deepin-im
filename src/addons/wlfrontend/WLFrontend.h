#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include "WaylandInputContextV2.h"
#include "WlType.h"

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

#include <memory>

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

    template<typename T>
    std::vector<std::shared_ptr<WlType<T>>> getGlobals()
    {
        uint32_t key = WlType<T>::key();
        auto iter = globals_.find(key);
        if (iter == globals_.end()) {
            return {};
        }

        auto m = iter->second;
        std::vector<std::shared_ptr<WlType<T>>> l;
        l.reserve(m.size());
        for (auto &it : m) {
            l.push_back(std::static_pointer_cast<WlType<T>>(it.second));
        }

        return l;
    }

    template<typename T>
    std::shared_ptr<WlType<T>> getGlobal()
    {
        auto v = getGlobals<T>();
        if (v.empty()) {
            return nullptr;
        }

        return *v.begin();
    }

private:
    static const wl_registry_listener registry_listener_;
    WaylandConnection *wl_;

    std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::shared_ptr<void>>> globals_;
    std::vector<std::shared_ptr<WaylandInputContextV2>> ims_;

    void
    global(struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
    void globalRemove(struct wl_registry *wl_registry, uint32_t name);

    void init();
    void reloadSeats();
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !DBUSFRONTEND_H

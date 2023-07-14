#ifndef DBUSFRONTEND_H
#define DBUSFRONTEND_H

#include "WaylandInputContextV2.h"
#include "wl/Type.h"

#include <dimcore/FrontendAddon.h>

#include <QDBusObjectPath>

#include <memory>

namespace wl {
class Connection;
}

struct wl_registry;
struct zwp_input_method_v2;
struct zwp_input_method_context_v2;

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

    // std::vector<std::shared_ptr<wl::Type<T>>> getGlobals()
    // {
    //     uint32_t key = wl::Type<T>::key();
    //     auto iter = globals_.find(key);
    //     if (iter == globals_.end()) {
    //         return {};
    //     }

    //     auto m = iter->second;
    //     std::vector<std::shared_ptr<wl::Type<T>>> l;
    //     l.reserve(m.size());
    //     for (auto &it : m) {
    //         l.push_back(std::static_pointer_cast<wl::Type<T>>(it.second));
    //     }

    //     return l;
    // }

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

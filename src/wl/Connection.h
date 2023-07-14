#ifndef WL_CONNECTION_H
#define WL_CONNECTION_H

#include "Type.h"
#include "common/common.h"

#include <wayland-client-protocol.h>

#include <QSocketNotifier>

#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace wl {

struct globalInfo
{
    uint32_t version;
    std::set<uint32_t> names;
};

class Connection : public QObject
{
public:
    Connection(const std::string &name, QObject *parent = nullptr);
    ~Connection();

    wl_display *display() const { return display_.get(); }

    void roundtrip();
    void flush();

    template<typename T>
    std::vector<std::shared_ptr<Type<T>>> getGlobals()
    {
        const std::string &interface = Type<T>::interface;
        auto iter = bindedGlobals_.find(interface);
        if (iter == bindedGlobals_.end()) {
            // bind
            auto gIter = globals_.find(interface);
            if (gIter == globals_.end()) {
                return {};
            }

            uint32_t version = gIter->second.version;
            for (auto &name : gIter->second.names) {
                auto *g = static_cast<T *>(wl_registry_bind(wl_display_get_registry(display_.get()),
                                                            name,
                                                            Type<T>::wl_interface,
                                                            version));
                std::shared_ptr<void> ptr = std::make_shared<Type<T>>(g);
                bindedGlobals_[interface].emplace(name, ptr);
            }

            iter = bindedGlobals_.find(interface);
        }

        auto &tmp = iter->second;
        std::vector<std::shared_ptr<Type<T>>> res;
        res.reserve(tmp.size());

        std::transform(tmp.cbegin(), tmp.cend(), std::back_inserter(res), [](const auto &pair) {
            return std::static_pointer_cast<Type<T>>(pair.second);
        });

        return res;
    }

    template<typename T>
    std::shared_ptr<Type<T>> getGlobal()
    {
        auto list = getGlobals<T>();
        if (list.empty()) {
            return nullptr;
        }

        return list[0];
    }

private:
    std::unique_ptr<wl_display, Deleter<&wl_display_disconnect>> display_;
    int fd_;
    QSocketNotifier *notifier_;

    static const wl_registry_listener registry_listener_;
    std::unordered_map<std::string, globalInfo> globals_;
    std::unordered_map<std::string, std::unordered_map<uint32_t, std::shared_ptr<void>>>
        bindedGlobals_;

    void init();
    void dispatch();

    void
    onGlobal(struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
    void onGlobalRemove(struct wl_registry *wl_registry, uint32_t name);
};

} // namespace wl

#endif // !WL_CONNECTION_H

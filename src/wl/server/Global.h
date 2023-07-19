#ifndef WL_SERVER_GLOBAL_H
#define WL_SERVER_GLOBAL_H

#include "Server.h"

#include <functional>
#include <memory>

namespace wl {
namespace server {

template<auto F>
struct GlobalCallbackWrapper;

template<typename C, typename R, typename Arg, typename... Args, R (C::*F)(Arg, Args...)>
struct GlobalCallbackWrapper<F>
{
    static R func(Arg farg, void *userdata, Args... args)
    {
        auto *p = reinterpret_cast<C *>(userdata);
        return (p->*F)(farg, args...);
    }
};

class Global
{
public:
    Global(const std::shared_ptr<Server> &server,
           const struct wl_interface *interface,
           const std::function<void(struct wl_client *client, uint32_t version, uint32_t id)>
               &onBindCallback);

private:
    std::unique_ptr<wl_global, Deleter<wl_global_destroy>> global_;
    const std::function<void(struct wl_client *client, uint32_t version, uint32_t id)>
        onBindCallback_;

    void onBind(struct wl_client *client, uint32_t version, uint32_t id);
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_GLOBAL_H

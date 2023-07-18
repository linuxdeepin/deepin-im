#include "Global.h"

using namespace wl;

Global::Global(const std::shared_ptr<Server> &server, const wl_interface *interface)
    : global_(wl_global_create(
        server->display(), interface, 1, this, GlobalCallbackWrapper<&Global::onBind>::func))
{
}

void Global::onBind([[maybe_unused]] struct wl_client *client,
                    [[maybe_unused]] uint32_t version,
                    [[maybe_unused]] uint32_t id)
{
}

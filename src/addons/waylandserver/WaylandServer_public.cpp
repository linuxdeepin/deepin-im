#include "WaylandServer_public.h"

#include "addons/waylandserver/WaylandServer.h"

namespace org::deepin::dim {

namespace waylandserver {

std::shared_ptr<wl_display> getRemote(Addon *addon)
{
    auto *wls = qobject_cast<WaylandServer *>(addon);
    return wls->getRemote();
}

std::shared_ptr<wl_display> getLocal(Addon *addon)
{
    auto *wls = qobject_cast<WaylandServer *>(addon);
    return wls->getLocal();
}

std::shared_ptr<wlr_backend> getBackend(Addon *addon)
{
    auto *wls = qobject_cast<WaylandServer *>(addon);
    return wls->getBackend();
}

} // namespace waylandserver

} // namespace org::deepin::dim

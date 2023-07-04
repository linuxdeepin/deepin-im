#include "InputContextProxy.h"

#include "WaylandConnection.h"
#include "utils.h"

#include <wayland-text-input-unstable-v3-client-protocol.h>

#include <QDebug>
#include <QList>

static const wl_registry_listener registry_listener = {
    CallbackWrapper<&InputContextProxy::registryGlobal>::func,
    []([[maybe_unused]] void *data, [[maybe_unused]] struct wl_registry *registry, uint32_t name) {
        qWarning() << "global_remove" << name;
    },
};

static const zwp_text_input_v3_listener tiListener = {
    CallbackWrapper<&InputContextProxy::enter>::func,
    CallbackWrapper<&InputContextProxy::leave>::func,
    CallbackWrapper<&InputContextProxy::preedit_string>::func,
    CallbackWrapper<&InputContextProxy::commit_string>::func,
    CallbackWrapper<&InputContextProxy::delete_surrounding_text>::func,
    CallbackWrapper<&InputContextProxy::done>::func,
};

InputContextProxy::InputContextProxy(QObject *parent)
    : QObject(parent)
    , available_(true)
{
    std::string waylandServer;
    const char *waylandServerCStr = getenv("WAYLAND_DISPLAY");
    if (waylandServerCStr == nullptr) {
        qWarning() << "WAYLAND_DISPLAY is empty";
        // TODO:
    }
    waylandServer = waylandServerCStr;

    wl_ = new WaylandConnection(waylandServer, this);

    auto *registry = wl_display_get_registry(wl_->display());
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_->roundTrip();
    wl_display_flush(wl_->display());

    zwp_text_input_v3_add_listener(text_input_v3_, &tiListener, this);
}

void InputContextProxy::focusIn()
{
    zwp_text_input_v3_enable(text_input_v3_);
}

void InputContextProxy::focusOut()
{
    zwp_text_input_v3_disable(text_input_v3_);
}

void InputContextProxy::processKeyEvent([[maybe_unused]] uint keyval,
                                        [[maybe_unused]] uint keycode,
                                        [[maybe_unused]] uint state,
                                        [[maybe_unused]] bool isRelease,
                                        [[maybe_unused]] uint time)
{
}

void InputContextProxy::registryGlobal(struct wl_registry *registry,
                                       uint32_t name,
                                       const char *interface,
                                       uint32_t version)
{
    qWarning() << "global" << name << interface << version;

#define BIND(member, interface_type)                                             \
  if (strcmp(interface, #interface_type) == 0) {                                 \
    member = static_cast<interface_type *>(                                      \
        wl_registry_bind(registry, name, &interface_type##_interface, version)); \
    return;                                                                      \
  }

    BIND(text_input_v3_, zwp_text_input_v3);
}

void InputContextProxy::enter([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                              [[maybe_unused]] struct wl_surface *surface)
{
}

void InputContextProxy::leave([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                              [[maybe_unused]] struct wl_surface *surface)
{
}

void InputContextProxy::preedit_string([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                                       const char *text,
                                       [[maybe_unused]] int32_t cursor_begin,
                                       [[maybe_unused]] int32_t cursor_end)
{
    QStringList data;
    data << text;
    // todo: split by cursor
    emit preedit(data);
}

void InputContextProxy::commit_string([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                                      const char *text)
{
    emit commitString(text);
}

void InputContextProxy::delete_surrounding_text(
    [[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
    [[maybe_unused]] uint32_t before_length,
    [[maybe_unused]] uint32_t after_length)
{
}

void InputContextProxy::done([[maybe_unused]] struct zwp_text_input_v3 *zwp_text_input_v3,
                             [[maybe_unused]] uint32_t serial)
{
}

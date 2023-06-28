#include "WLFrontend.h"

#include "InputContext1.h"
#include "WaylandConnection.h"

#include <dimcore/Dim.h>
#include <wayland-input-method-unstable-v1-client-protocol.h>

#include <QDBusConnection>
#include <QDebug>

using namespace org::deepin::dim;

WLFrontend::WLFrontend()
{
    std::string waylandDisplay = getenv("WAYLAND_DISPLAY");
    if (waylandDisplay.empty()) {
        qDebug("WAYLAND_DISPLAY is not set");
        // todo: fake wayland server
    }

    // zwp_input_method_context_v1_listener listener;
    // listener.surrounding_text =
    //     []([[maybe_unused]] void *data,
    //        [[maybe_unused]] struct zwp_input_method_context_v1 *zwp_input_method_context_v1,
    //        const char *text,
    //        uint32_t cursor,
    //        uint32_t anchor) {
    //         qDebug() << "surrounding_text" << text << cursor << anchor;
    //     };
    // listener.reset =
    //     []([[maybe_unused]] void *data,
    //        [[maybe_unused]] struct zwp_input_method_context_v1 *zwp_input_method_context_v1) {
    //         qDebug() << "reset";
    //     };
    // listener.content_type =
    //     []([[maybe_unused]] void *data,
    //        [[maybe_unused]] struct zwp_input_method_context_v1 *zwp_input_method_context_v1,
    //        uint32_t hint,
    //        uint32_t purpose) {
    //         qDebug() << "content_type" << hint << purpose;
    //     };
    // listener.invoke_action =
    //     []([[maybe_unused]] void *data,
    //        [[maybe_unused]] struct zwp_input_method_context_v1 *zwp_input_method_context_v1,
    //        uint32_t button,
    //        uint32_t index) {
    //         qDebug() << "invoke_action" << button << index;
    //     };
    // listener.commit_state =
    //     []([[maybe_unused]] void *data,
    //        [[maybe_unused]] struct zwp_input_method_context_v1 *zwp_input_method_context_v1,
    //        uint32_t serial) {
    //         qDebug() << "commit_state" << serial;
    //     };
    // listener.preferred_language =
    //     []([[maybe_unused]] void *data,
    //        [[maybe_unused]] struct zwp_input_method_context_v1 *zwp_input_method_context_v1,
    //        const char *language) {
    //         qDebug() << "preferred_language" << language;
    //     };
    // zwp_input_method_context_v1_add_listener(nullptr, &listener, nullptr);

    wl_ = new WaylandConnection(waylandDisplay, this);

    struct wl_registry_listener registry_listener;
    registry_listener.global = []([[maybe_unused]] void *data,
                                  [[maybe_unused]] struct wl_registry *registry,
                                  uint32_t name,
                                  const char *interface,
                                  uint32_t version) {
        qWarning() << "global" << name << interface << version;
    };
    registry_listener.global_remove = []([[maybe_unused]] void *data,
                                         [[maybe_unused]] struct wl_registry *registry,
                                         uint32_t name) {
        qWarning() << "global_remove" << name;
    };

    auto *registry = wl_display_get_registry(wl_->display());
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(wl_->display());

    // zwp_input_method_v1_listener listener;
    // listener.activate = []([[maybe_unused]] void *data,
    //                        [[maybe_unused]] struct zwp_input_method_v1 *zwp_input_method_v1,
    //                        [[maybe_unused]] struct zwp_input_method_context_v1 *id) {
    //     qDebug() << "activate";
    // };
    // listener.deactivate = []([[maybe_unused]] void *data,
    //                          [[maybe_unused]] struct zwp_input_method_v1 *zwp_input_method_v1,
    //                          [[maybe_unused]] struct zwp_input_method_context_v1 *id) {
    //     qDebug() << "deactivate";
    // };
    // zwp_input_method_v1_add_listener(nullptr, &listener, nullptr);
}

WLFrontend::~WLFrontend() { }

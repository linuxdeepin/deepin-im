#ifndef WAYLANDSERVER_H
#define WAYLANDSERVER_H

#include <stdint.h>

struct wl_global;
struct wl_display;

class WaylandServer
{
public:
    WaylandServer();
    ~WaylandServer();

    void run();

private:
    wl_display *display_;
    wl_global *text_input_manager_global_;
    wl_global *text_input_global_;
    wl_global *input_method_global_;
    wl_global *input_method_context_global_;

    void textInputManagerV3Bind(struct wl_client *client, uint32_t version, uint32_t id);
    void textInputV3Bind(struct wl_client *client, uint32_t version, uint32_t id);
    void inputMethodV1Bind(struct wl_client *client, uint32_t version, uint32_t id);
    void inputMethodContextV1Bind(struct wl_client *client, uint32_t version, uint32_t id);
};

#endif // !WAYLANDSERVER_H

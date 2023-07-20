#ifndef WL_SERVER_ZWPTEXTINPUTV3_H
#define WL_SERVER_ZWPTEXTINPUTV3_H

#include "Type.h"
#include "wayland-text-input-unstable-v3-server-protocol.h"

namespace wl {
namespace server {

class ZwpTextInputV3 : public BASE_TYPE(zwp_text_input_v3)
{
    friend class Type;

public:
    ZwpTextInputV3(struct wl_client *client, uint32_t id);

    void destroy(struct wl_client *client);
    void enable(struct wl_client *client);
    void disable(struct wl_client *client);
    void
    setSurroundingText(struct wl_client *client, const char *text, int32_t cursor, int32_t anchor);
    void setTextChangeCause(struct wl_client *client, uint32_t cause);
    void setContentType(struct wl_client *client, uint32_t hint, uint32_t purpose);
    void setCursorRectangle(
        struct wl_client *client, int32_t x, int32_t y, int32_t width, int32_t height);
    void commit(struct wl_client *client);

    void resourceDestroy() override;
};

} // namespace server
} // namespace wl

#endif // !WL_SERVER_ZWPTEXTINPUTV3_H

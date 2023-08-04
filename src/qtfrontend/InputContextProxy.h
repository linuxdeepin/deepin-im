#ifndef INPUTCONTEXTPROXY_H
#define INPUTCONTEXTPROXY_H

#include "wayland-text-input-unstable-v3-client-protocol.h"

#include <QObject>

namespace wl {
namespace client {
class ConnectionFd;
class ZwpTextInputV3;
} // namespace client
} // namespace wl

struct zwp_text_input_v3;

class InputContextProxy : public QObject
{
    Q_OBJECT

public:
    InputContextProxy(QObject *parent = nullptr);

    bool available() const { return available_; }

    void focusIn();
    void focusOut();
    void processKeyEvent(uint keyval, uint keycode, uint state, bool isRelease, uint time);

protected:
    // wayland listeners
    void enter(struct zwp_text_input_v3 *zwp_text_input_v3, struct wl_surface *surface);
    void leave(struct zwp_text_input_v3 *zwp_text_input_v3, struct wl_surface *surface);
    void preedit_string(struct zwp_text_input_v3 *zwp_text_input_v3,
                        const char *text,
                        int32_t cursor_begin,
                        int32_t cursor_end);
    void commit_string(struct zwp_text_input_v3 *zwp_text_input_v3, const char *text);
    void delete_surrounding_text(struct zwp_text_input_v3 *zwp_text_input_v3,
                                 uint32_t before_length,
                                 uint32_t after_length);
    void done(struct zwp_text_input_v3 *zwp_text_input_v3, uint32_t serial);

signals:
    void preedit(const QList<QString> &preedit);
    void commitString(const QString &text);
    void forwardKey(uint32_t keyValue, uint32_t state, bool type);

private:
    wl::client::ConnectionFd *wl_;
    bool available_;
    std::shared_ptr<wl::client::ZwpTextInputV3> ti_;
    static const zwp_text_input_v3_listener tiListener;
};

#endif // !INPUTCONTEXTPROXY_H

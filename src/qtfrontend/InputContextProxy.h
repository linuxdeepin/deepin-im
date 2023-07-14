#ifndef INPUTCONTEXTPROXY_H
#define INPUTCONTEXTPROXY_H

#include <QObject>

namespace wl {
class Connection;
}

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

    // wayland listeners
    void registryGlobal(struct wl_registry *registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version);
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
    wl::Connection *wl_;
    struct zwp_text_input_v3 *text_input_v3_;
    bool available_;
};

#endif // !INPUTCONTEXTPROXY_H

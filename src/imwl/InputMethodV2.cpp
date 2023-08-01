#include "InputMethodV2.h"

#include "Core.h"
#include "InputMethodKeyboardGrabV2.h"
#include "TextInputManagerV3.h"
#include "TextInputV3.h"
#include "common.h"
#include "qwayland-server-input-method-unstable-v2.h"

class InputMethodV2Private : public QtWaylandServer::zwp_input_method_v2
{
public:
    InputMethodV2Private(InputMethodV2 *q)
        : q(q)
    {
    }

    ~InputMethodV2Private() { }

protected:
    void zwp_input_method_v2_commit_string(Resource *resource, const QString &text) override
    {
        auto *im = getTextInputV3();
        im->sendCommitString(text);
    }

    void zwp_input_method_v2_set_preedit_string(Resource *resource,
                                                const QString &text,
                                                int32_t cursor_begin,
                                                int32_t cursor_end) override
    {
        auto *im = getTextInputV3();
        im->sendPreeditString(text, cursor_begin, cursor_end);
    }

    void zwp_input_method_v2_delete_surrounding_text(Resource *resource,
                                                     uint32_t before_length,
                                                     uint32_t after_length) override
    {
    }

    void zwp_input_method_v2_commit(Resource *resource, uint32_t serial) override
    {
        auto *im = getTextInputV3();
        im->sendDone(serial);
    }

    void zwp_input_method_v2_get_input_popup_surface(Resource *resource,
                                                     uint32_t id,
                                                     struct ::wl_resource *surface) override
    {
    }

    void zwp_input_method_v2_grab_keyboard(Resource *resource, uint32_t keyboard) override
    {
        q->m_grab->add(resource->client(), keyboard);
    }

    void zwp_input_method_v2_destroy(Resource *resource) override
    {
        wl_resource_destroy(resource->handle);
    }

private:
    InputMethodV2 *q;

    TextInputV3 *getTextInputV3()
    {
        return q->m_core->getTextInputManagerV3()->getTextInputV4BySeat(q->m_seat);
    }
};

InputMethodV2::InputMethodV2(Core *core, struct ::wl_resource *seat, QObject *parent)
    : QObject(parent)
    , m_core(core)
    , m_seat(seat)
    , m_grab(new InputMethodKeyboardGrabV2(seat, this))
{
}

InputMethodV2::~InputMethodV2() { }

INIT_FUNCS(InputMethodV2)

void InputMethodV2::sendDeactivate()
{
    const auto resources = d->resourceMap();
    for (auto *resource : resources) {
        d->send_deactivate(resource->handle);
    }
}

void InputMethodV2::sendActivate()
{
    const auto resources = d->resourceMap();
    for (auto *resource : resources) {
        d->send_activate(resource->handle);
    }
}

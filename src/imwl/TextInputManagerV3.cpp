#include "TextInputManagerV3.h"

#include "TextInputV3.h"
#include "qwayland-server-text-input-unstable-v3.h"

#include <QtWaylandCompositor/QWaylandSeat>

class TextInputManagerV3Private : public QtWaylandServer::zwp_text_input_manager_v3
{
public:
    TextInputManagerV3Private(TextInputManagerV3 *q)
        : q(q)
    {
    }

protected:
    void zwp_text_input_manager_v3_destroy(Resource *resource) override
    {
        wl_resource_destroy(resource->handle);
    }

    void zwp_text_input_manager_v3_get_text_input(Resource *resource,
                                                  uint32_t id,
                                                  struct ::wl_resource *seat) override
    {
        auto iter = q->m_textInputs.find(seat);
        if (iter == q->m_textInputs.end()) {
            auto *ti = new TextInputV3(q->m_core, seat, q);
            auto [i, r] = q->m_textInputs.emplace(seat, ti);
            iter = i;
        }

        iter->second->add(resource->client(), id);
    }

private:
    TextInputManagerV3 *q;
};

TextInputManagerV3::TextInputManagerV3(Core *core, QObject *parent)
    : QObject(parent)
    , m_core(core)
    , d(new TextInputManagerV3Private(this))
{
}

TextInputManagerV3::~TextInputManagerV3() { }

INIT_FUNCS(TextInputManagerV3)

TextInputV3 *TextInputManagerV3::getTextInputV4BySeat(struct ::wl_resource *seat)
{
    return m_textInputs.at(seat);
}
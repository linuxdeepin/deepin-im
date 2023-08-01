#include "TextInputV3.h"

#include "Core.h"
#include "InputMethodManagerV2.h"
#include "InputMethodV2.h"
#include "qwayland-server-text-input-unstable-v3.h"

#include <set>

class TextInputV3Private : public QtWaylandServer::zwp_text_input_v3
{
    friend class TextInputV3;

public:
    TextInputV3Private(TextInputV3 *q)
        : q(q)
    {
    }

    ~TextInputV3Private() { }

protected:
    void zwp_text_input_v3_destroy(Resource *resource) override
    {
        m_enabled.erase(resource);

        wl_resource_destroy(resource->handle);
    }

    void zwp_text_input_v3_enable(Resource *resource) override
    {
        m_enabled.emplace(resource);

        auto *im = q->m_core->getInputMethodManagerV2()->getInputMethodV2BySeat(q->m_seat);
        im->sendActivate();
    }

    void zwp_text_input_v3_disable(Resource *resource) override
    {
        m_enabled.erase(resource);

        auto *im = q->m_core->getInputMethodManagerV2()->getInputMethodV2BySeat(q->m_seat);
        im->sendDeactivate();
    }

    void zwp_text_input_v3_set_surrounding_text(Resource *resource,
                                                const QString &text,
                                                int32_t cursor,
                                                int32_t anchor) override
    {
    }

    void zwp_text_input_v3_set_text_change_cause(Resource *resource, uint32_t cause) override { }

    void zwp_text_input_v3_set_content_type(Resource *resource,
                                            uint32_t hint,
                                            uint32_t purpose) override
    {
    }

    void zwp_text_input_v3_set_cursor_rectangle(
        Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) override
    {
    }

    void zwp_text_input_v3_commit(Resource *resource) override { }

private:
    TextInputV3 *q;
    std::set<Resource *> m_enabled;
};

TextInputV3::TextInputV3(Core *core, struct ::wl_resource *seat, QObject *parent)
    : QObject(parent)
    , m_core(core)
    , d(new TextInputV3Private(this))
{
}

TextInputV3::~TextInputV3() { }

INIT_FUNCS(TextInputV3)

void TextInputV3::sendPreeditString(const QString &text, int32_t cursor_begin, int32_t cursor_end)
{
    const auto resources = d->resourceMap();
    for (auto *resource : resources) {
        if (d->m_enabled.find(resource) == d->m_enabled.end()) {
            continue;
        }
        d->send_preedit_string(resource->handle, text, cursor_begin, cursor_end);
    }
}

void TextInputV3::sendCommitString(const QString &text)
{
    const auto resources = d->resourceMap();
    for (auto *resource : resources) {
        if (d->m_enabled.find(resource) == d->m_enabled.end()) {
            continue;
        }
        d->send_commit_string(text);
    }
}

void TextInputV3::sendDone(uint32_t serial)
{
    const auto resources = d->resourceMap();
    for (auto *resource : resources) {
        if (d->m_enabled.find(resource) == d->m_enabled.end()) {
            continue;
        }
        d->send_done(resource->handle, serial);
    }
}

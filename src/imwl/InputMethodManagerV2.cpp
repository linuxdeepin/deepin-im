#include "InputMethodManagerV2.h"

#include "InputMethodV2.h"
#include "common.h"
#include "qwayland-server-input-method-unstable-v2.h"

class InputMethodManagerV2Private : public QtWaylandServer::zwp_input_method_manager_v2
{
public:
    InputMethodManagerV2Private(InputMethodManagerV2 *q)
        : q(q)
    {
    }

    ~InputMethodManagerV2Private() { }

protected:
    void zwp_input_method_manager_v2_get_input_method(Resource *resource,
                                                      struct ::wl_resource *seat,
                                                      uint32_t input_method) override
    {
        auto iter = q->m_inputmethods.find(seat);
        if (iter == q->m_inputmethods.end()) {
            auto *im = new InputMethodV2(q->m_core, seat, q);
            auto [i, r] = q->m_inputmethods.emplace(seat, im);
            iter = i;
        }

        iter->second->add(resource->client(), input_method);
    }

    void zwp_input_method_manager_v2_destroy(Resource *resource) override { }

private:
    InputMethodManagerV2 *q;
};

InputMethodManagerV2::InputMethodManagerV2(Core *core, QObject *parent)
    : QObject(parent)
    , m_core(core)
    , d(new InputMethodManagerV2Private(this))
{
}

InputMethodManagerV2::~InputMethodManagerV2() { }

INIT_FUNCS(InputMethodManagerV2)

InputMethodV2 *InputMethodManagerV2::getInputMethodV2BySeat(struct ::wl_resource *seat)
{
    return m_inputmethods.at(seat);
}

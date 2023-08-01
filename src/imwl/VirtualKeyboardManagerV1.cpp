#include "VirtualKeyboardManagerV1.h"

#include "VirtualKeyboardV1.h"
#include "qwayland-server-virtual-keyboard-unstable-v1.h"

#include <QWaylandKeymap>
#include <QWaylandSeat>

class VirtualKeyboardManagerV1Private : public QtWaylandServer::zwp_virtual_keyboard_manager_v1
{
public:
    VirtualKeyboardManagerV1Private(VirtualKeyboardManagerV1 *q)
        : q(q)
    {
    }

protected:
    void zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(Resource *resource,
                                                                 struct ::wl_resource *seat,
                                                                 uint32_t id) override
    {
        auto *s = QWaylandSeat::fromSeatResource(seat);

        auto iter = q->m_virtualKeyboards.find(seat);
        if (iter == q->m_virtualKeyboards.end()) {
            auto *vk = new VirtualKeyboardV1(seat, q);
            auto [i, r] = q->m_virtualKeyboards.emplace(seat, vk);
            iter = i;
        }

        iter->second->add(resource->client(), id);
    }

private:
    VirtualKeyboardManagerV1 *q;
};

VirtualKeyboardManagerV1::VirtualKeyboardManagerV1(QObject *paernt)
    : QObject(paernt)
    , d(new VirtualKeyboardManagerV1Private(this))
{
}

VirtualKeyboardManagerV1::~VirtualKeyboardManagerV1() { }

INIT_FUNCS(VirtualKeyboardManagerV1)

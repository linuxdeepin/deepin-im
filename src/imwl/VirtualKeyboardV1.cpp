// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "VirtualKeyboardV1.h"

#include "qwayland-server-virtual-keyboard-unstable-v1.h"

#include <xkbcommon/xkbcommon.h>

#include <QKeyEvent>
#include <QWaylandKeymap>
#include <QWaylandSeat>

#include <sys/mman.h>

class VirtualKeyboardV1Private : public QtWaylandServer::zwp_virtual_keyboard_v1
{
public:
    VirtualKeyboardV1Private(VirtualKeyboardV1 *q)
        : q(q)
        , m_xkbContext(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
    {
    }

    ~VirtualKeyboardV1Private() { }

protected:
    void zwp_virtual_keyboard_v1_keymap(Resource *resource,
                                        uint32_t format,
                                        int32_t fd,
                                        uint32_t size) override
    {
        void *ptr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (ptr == MAP_FAILED) {
            return;
        }

        m_xkbKeymap.reset(xkb_keymap_new_from_string(m_xkbContext.get(),
                                                     static_cast<const char *>(ptr),
                                                     XKB_KEYMAP_FORMAT_TEXT_V1,
                                                     XKB_KEYMAP_COMPILE_NO_FLAGS));
        munmap(ptr, size);

        if (!m_xkbKeymap) {
            return;
        }

        m_xkbState.reset(xkb_state_new(m_xkbKeymap.get()));
        if (!m_xkbState) {
            m_xkbKeymap.reset();
            return;
        }
    }

    void zwp_virtual_keyboard_v1_key(Resource *resource,
                                     uint32_t time,
                                     uint32_t key,
                                     uint32_t state) override
    {
        q->seat_->sendKeyEvent(key, state);
    }

    void zwp_virtual_keyboard_v1_modifiers(Resource *resource,
                                           uint32_t mods_depressed,
                                           uint32_t mods_latched,
                                           uint32_t mods_locked,
                                           uint32_t group) override
    {
    }

    void zwp_virtual_keyboard_v1_destroy(Resource *resource) override
    {
        wl_resource_destroy(resource->handle);
    }

private:
    VirtualKeyboardV1 *q;

    std::unique_ptr<xkb_context, Deleter<xkb_context_unref>> m_xkbContext;
    std::unique_ptr<xkb_keymap, Deleter<xkb_keymap_unref>> m_xkbKeymap;
    std::unique_ptr<xkb_state, Deleter<xkb_state_unref>> m_xkbState;
};

VirtualKeyboardV1::VirtualKeyboardV1(struct ::wl_resource *seat, QObject *parent)
    : QObject(parent)
    , d(new VirtualKeyboardV1Private(this))
    , seat_(QWaylandSeat::fromSeatResource(seat))
{
}

VirtualKeyboardV1::~VirtualKeyboardV1() { }

INIT_FUNCS(VirtualKeyboardV1)

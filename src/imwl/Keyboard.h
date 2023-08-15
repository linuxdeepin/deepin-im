#ifndef IMWL_KEYBOARD_H
#define IMWL_KEYBOARD_H

#include "wl/server/Keyboard.h"

class Seat;

class Keyboard : public wl::server::Keyboard {
public:
    Keyboard(Seat *seat);
    virtual ~Keyboard();

    void sendKeymap(uint32_t format, int32_t fd, uint32_t size);
    void sendKey(uint32_t serial, uint32_t time, uint32_t key, uint32_t state);

protected:
    void wl_keyboard_release(wl::server::Resource *resource) override;

private:
    Seat *seat_;
};

#endif // !IMWL_KEYBOARD_H

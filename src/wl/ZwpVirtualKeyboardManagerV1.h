#ifndef ZWPVIRTUALKEYBOARDMANAGERV1_H
#define ZWPVIRTUALKEYBOARDMANAGERV1_H

#include "Type.h"

#include <memory>

namespace wl {

class Seat;
class ZwpVirtualKeyboardV1;

class ZwpVirtualKeyboardManagerV1 : public Type<zwp_virtual_keyboard_manager_v1>
{
public:
    ZwpVirtualKeyboardManagerV1(zwp_virtual_keyboard_manager_v1 *val);
    ~ZwpVirtualKeyboardManagerV1();

    std::shared_ptr<ZwpVirtualKeyboardV1> createVirtualKeyboard(const std::shared_ptr<Seat> &seat);
};

} // namespace wl

#endif // !ZWPVIRTUALKEYBOARDMANAGERV1_H

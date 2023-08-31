// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMWL_SEAT_H
#define IMWL_SEAT_H

#include "wl/server/Seat.h"

class Keyboard;
class DimTextInputV1;
class TextInputV3;
class InputMethodV2;
class VirtualKeyboardV1;

class Seat : public wl::server::Seat
{
public:
    Seat();
    virtual ~Seat();

    std::shared_ptr<Keyboard> getKeyboard() { return keyboard_; }

    std::shared_ptr<DimTextInputV1> getDimTextInputV1() { return dimTextInputV1_; }

    std::shared_ptr<TextInputV3> getTextInputV3() { return textInputV3_; }

    std::shared_ptr<InputMethodV2> getInputMethodV2() { return inputMethodV2_; }

    std::shared_ptr<VirtualKeyboardV1> getVirtualKeyboardV1() { return virtualKeyboardV1_; }

protected:
    void wl_seat_get_pointer(wl::server::Resource *resource, uint32_t id) override;
    void wl_seat_get_keyboard(wl::server::Resource *resource, uint32_t id) override;
    void wl_seat_get_touch(wl::server::Resource *resource, uint32_t id) override;
    void wl_seat_release(wl::server::Resource *resource) override;

private:
    std::shared_ptr<Keyboard> keyboard_;
    std::shared_ptr<DimTextInputV1> dimTextInputV1_;
    std::shared_ptr<TextInputV3> textInputV3_;
    std::shared_ptr<InputMethodV2> inputMethodV2_;
    std::shared_ptr<VirtualKeyboardV1> virtualKeyboardV1_;
};

#endif // !IMWL_SEAT_H


// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTPOPUPSURFACEV2_H
#define INPUTPOPUPSURFACEV2_H

#include "wl/server/ZwpInputPopupSurfaceV2.h"

#include <memory>

namespace wl {
namespace server {
class Seat;
}
} // namespace wl

class Seat;
class TextInputV3;
class InputPopupSurfaceV2;
class InputMethodKeyboardGrabV2;

class InputPopupSurfaceV2 : public wl::server::ZwpInputPopupSurfaceV2
{
public:
    InputPopupSurfaceV2();
    ~InputPopupSurfaceV2() override;

    void sendTextInputRectangle(int32_t x, int32_t y, int32_t width, int32_t height);

protected:
    void zwp_input_popup_surface_v2_destroy(wl::server::Resource *resource) override;

private:
};

#endif // !INPUTPOPUPSURFACEV2_H

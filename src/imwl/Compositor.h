// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "wl/server/Server.h"

#include <memory>

class QSocketNotifier;

class Seat;
class DimTextInputManagerV1;
class TextInputManagerV3;
class InputMethodManagerV2;
class VirtualKeyboardManagerV1;

class Compositor : public wl::server::Server
{
public:
    Compositor();
    ~Compositor();

    void create();

private:
    std::unique_ptr<QSocketNotifier> noti_;

    std::unique_ptr<Seat> seat_;
    std::unique_ptr<DimTextInputManagerV1> dimTextInputManagerV1_;
    // std::unique_ptr<TextInputManagerV3> textInputManagerV3_;
    std::unique_ptr<InputMethodManagerV2> inputMethodManagerV2_;
    std::unique_ptr<VirtualKeyboardManagerV1> virtualKeyboardManagerV1_;
};

#endif // COMPOSITOR_H

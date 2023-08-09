// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "Core.h"
#include "wl/server/Server.h"

#include <memory>

class QSocketNotifier;

class TextInputManagerV3;
class InputMethodManagerV2;
class VirtualKeyboardManagerV1;

class Compositor : public wl::server::Server, public Core
{
public:
    Compositor();
    ~Compositor();

    void create();

    TextInputManagerV3 *getTextInputManagerV3() override;
    InputMethodManagerV2 *getInputMethodManagerV2() override;

private:
    std::unique_ptr<QSocketNotifier> noti_;

    TextInputManagerV3 *textInputManagerV3_;
    InputMethodManagerV2 *inputMethodManagerV2_;
    VirtualKeyboardManagerV1 *virtualKeyboardManagerV1_;
};

#endif // COMPOSITOR_H

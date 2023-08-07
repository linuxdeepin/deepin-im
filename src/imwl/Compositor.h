// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "Core.h"

#include <QtWaylandCompositor/QWaylandCompositor>

class TextInputManagerV3;
class InputMethodManagerV2;
class VirtualKeyboardManagerV1;

class Compositor : public QWaylandCompositor, public Core
{
    Q_OBJECT

public:
    Compositor();
    ~Compositor() override;

    void create() override;

    TextInputManagerV3 *getTextInputManagerV3() override;
    InputMethodManagerV2 *getInputMethodManagerV2() override;

private:
    TextInputManagerV3 *textInputManagerV3_;
    InputMethodManagerV2 *inputMethodManagerV2_;
    VirtualKeyboardManagerV1 *virtualKeyboardManagerV1_;
};

#endif // COMPOSITOR_H

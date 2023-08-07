// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Compositor.h"

#include "InputMethodManagerV2.h"
#include "TextInputManagerV3.h"
#include "VirtualKeyboardManagerV1.h"

Compositor::Compositor()
    : QWaylandCompositor()
{
}

Compositor::~Compositor() { }

void Compositor::create()
{
    QWaylandCompositor::create();

    textInputManagerV3_ = new TextInputManagerV3(this, this);
    textInputManagerV3_->init(display());

    inputMethodManagerV2_ = new InputMethodManagerV2(this, this);
    inputMethodManagerV2_->init(display());

    virtualKeyboardManagerV1_ = new VirtualKeyboardManagerV1(this);
    virtualKeyboardManagerV1_->init(display());
}

TextInputManagerV3 *Compositor::getTextInputManagerV3()
{
    return textInputManagerV3_;
}

InputMethodManagerV2 *Compositor::getInputMethodManagerV2()
{
    return inputMethodManagerV2_;
}

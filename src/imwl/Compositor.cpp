// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

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

    m_textInputManagerV3 = new TextInputManagerV3(this, this);
    m_textInputManagerV3->init(display());

    m_inputMethodManagerV2 = new InputMethodManagerV2(this, this);
    m_inputMethodManagerV2->init(display());

    m_virtualKeyboardManagerV1 = new VirtualKeyboardManagerV1(this);
    m_virtualKeyboardManagerV1->init(display());
}

TextInputManagerV3 *Compositor::getTextInputManagerV3()
{
    return m_textInputManagerV3;
}

InputMethodManagerV2 *Compositor::getInputMethodManagerV2()
{
    return m_inputMethodManagerV2;
}

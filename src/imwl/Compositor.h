// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "X11ActiveWindowMonitor.h"

#include <QtWaylandCompositor/QWaylandCompositor>

#include <memory>

class Seat;
class DimTextInputManagerV1;
class TextInputManagerV3;
class InputMethodManagerV2;
class VirtualKeyboardManagerV1;
class DimTextInputV1;
class TextInputV3;
class InputMethodV2;
class VirtualKeyboardV1;

class Compositor : public QWaylandCompositor
{
public:
    Compositor();
    ~Compositor();

    void create();

    std::shared_ptr<DimTextInputV1> getDimTextInputV1() { return dimTextInputV1_; }

    std::shared_ptr<TextInputV3> getTextInputV3() { return textInputV3_; }

    std::shared_ptr<InputMethodV2> getInputMethodV2() { return inputMethodV2_; }

    std::shared_ptr<VirtualKeyboardV1> getVirtualKeyboardV1() { return virtualKeyboardV1_; }

private:
    std::unique_ptr<X11ActiveWindowMonitor> activeWindowMonitor_;

    std::unique_ptr<DimTextInputManagerV1> dimTextInputManagerV1_;
    // std::unique_ptr<TextInputManagerV3> textInputManagerV3_;
    std::unique_ptr<InputMethodManagerV2> inputMethodManagerV2_;
    std::unique_ptr<VirtualKeyboardManagerV1> virtualKeyboardManagerV1_;

    std::shared_ptr<DimTextInputV1> dimTextInputV1_;
    std::shared_ptr<TextInputV3> textInputV3_;
    std::shared_ptr<InputMethodV2> inputMethodV2_;
    std::shared_ptr<VirtualKeyboardV1> virtualKeyboardV1_;

    pid_t activePid_;

    void activeWindowChanged(pid_t pid);
};

#endif // COMPOSITOR_H

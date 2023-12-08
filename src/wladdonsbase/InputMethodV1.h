// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INPUTMETHODV1_H
#define INPUTMETHODV1_H

#include "Listener.h"
#include "inputmethodv1/ZwpInputMethodV1.h"

#include <functional>
#include <memory>

WL_ADDONS_BASE_BEGIN_NAMESPACE

class Server;

class InputMethodV1
{
    friend class Keyboard;

public:
    InputMethodV1(Server *server, ZwpInputMethodV1 *input_method);
    ~InputMethodV1();

    void setCommitCallback(const std::function<void()> &callback) { commitCallback_ = callback; }
    void setPanelCreateCallback(const std::function<void()> &callback) { panelCreateCallback_ = callback; }
    void setPanelDestroyCallback(const std::function<void()> &callback) { panelDestroyCallback_ = callback; }

    void sendActivate();
    void sendDeactivate();
    void sendContentType(uint32_t hint, uint32_t purpose);
    void sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor);
    void sendDone();
    void setCursorRectangle(int x, int y, int width, int height);
    void sendKey(uint32_t keycode, bool isRelease);

private:
    Server *server_;
    ZwpInputMethodV1 *input_method_;

    std::function<void()> commitCallback_;
    std::function<void()> panelCreateCallback_;
    std::function<void()> panelDestroyCallback_;
};

WL_ADDONS_BASE_END_NAMESPACE

#endif // !INPUTMETHODV1_H

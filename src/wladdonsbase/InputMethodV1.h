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
class ZwpInputMethodContextV1;
class ZwpInputPanelV1;
class ZwpInputPanelSurfaceV1;

typedef std::function<void()> Cb;
typedef std::function<void(uint32_t serial, const char *text)> commitCb;
typedef std::function<void(uint32_t serial, const char *text, const char *commit)> preeditCb;
typedef std::function<void(uint32_t key, uint32_t state)> forwardKeyCb;

class InputMethodV1 : public ZwpInputMethodV1
{
    friend class ZwpInputPanelV1;
    friend class ZwpInputPanelSurfaceV1;
    friend class ZwpInputMethodContextV1;

public:
    explicit InputMethodV1(Server *server);
    ~InputMethodV1();

    void setCommitCallback(const commitCb &callback) { commitCallback_ = callback; }

    void setPreeditCallback(const preeditCb &callback) { preeditCallback_ = callback; }

    void setForwardKeyCallback(const forwardKeyCb &callback) { forwardKeyCallback_ = callback; }

    void setPanelCreateCallback(const Cb &callback) { panelCreateCallback_ = callback; }

    void setPanelDestroyCallback(const Cb &callback) { panelDestroyCallback_ = callback; }

    void sendActivate();
    void sendDeactivate();
    void sendContentType(uint32_t hint, uint32_t purpose);
    void sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor);
    void setCursorRectangle(int x, int y, int width, int height);
    void sendKey(uint32_t keycode, bool isRelease);

private:
    bool isInputMethodContextResourceValid() const;

private:
    Server *server_;
    std::shared_ptr<ZwpInputMethodContextV1> inputMethodContext_;
    std::shared_ptr<ZwpInputPanelV1> inputPanelV1_;

    commitCb commitCallback_;
    preeditCb preeditCallback_;
    forwardKeyCb forwardKeyCallback_;
    Cb panelCreateCallback_;
    Cb panelDestroyCallback_;
};

WL_ADDONS_BASE_END_NAMESPACE

#endif // !INPUTMETHODV1_H

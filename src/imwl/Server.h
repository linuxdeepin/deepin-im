// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERVER_H
#define SERVER_H

#include "X11ActiveWindowMonitor.h"
#include "wl/server/Server.h"

#include <memory>

class QSocketNotifier;

class Compositor;
class Seat;
class DimTextInputManagerV1;
class TextInputManagerV3;
class InputMethodManagerV2;
class VirtualKeyboardManagerV1;

class Server : public wl::server::Server
{
public:
    Server();
    ~Server();

    void create();

private:
    std::unique_ptr<X11ActiveWindowMonitor> activeWindowMonitor_;
    std::unique_ptr<QSocketNotifier> noti_;

    std::unique_ptr<Compositor> compositor_;
    std::unique_ptr<Seat> seat_;
    std::unique_ptr<DimTextInputManagerV1> dimTextInputManagerV1_;
    // std::unique_ptr<TextInputManagerV3> textInputManagerV3_;
    std::unique_ptr<InputMethodManagerV2> inputMethodManagerV2_;
    std::unique_ptr<VirtualKeyboardManagerV1> virtualKeyboardManagerV1_;

    pid_t activePid_;

    void activeWindowChanged(pid_t pid);
};

#endif // SERVER_H

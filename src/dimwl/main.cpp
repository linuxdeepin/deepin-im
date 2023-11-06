// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Server.h"

#include <iostream>

int main()
{
    Server dimwl;
    std::string socket = dimwl.addSocketAuto();
    std::cout << socket << std::endl;
    dimwl.startBackend();
    dimwl.run();
}

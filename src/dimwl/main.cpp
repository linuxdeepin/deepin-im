#include "Server.h"

#include <wlr/backend.h>

#include <iostream>

int main()
{
    Server dimwl;
    std::string socket = dimwl.addSocketAuto();
    std::cout << socket << std::endl;
    dimwl.startBackend();
    dimwl.run();
}

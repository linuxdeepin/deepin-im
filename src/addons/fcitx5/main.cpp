#include "Server.h"

int main() {
    Server server(nullptr, nullptr);
    server.addSocket("dimfcitx5");
    server.run();
}

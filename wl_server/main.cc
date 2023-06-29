#include <wayland-server.h>

#include <filesystem>
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main()
{
    struct wl_display *display = wl_display_create();
    if (!display) {
        std::cerr << "Unable to create Wayland display" << std::endl;
        return 1;
    }

    int ret = wl_display_add_socket(display, "imfakewl");
    if (ret == -1) {
        std::cerr << "Unable to add socket to Wayland display" << std::endl;
        return 1;
    }

    std::cout << "Running Wayland display" << std::endl;
    wl_display_run(display);

    wl_display_destroy(display);

    return 0;
}

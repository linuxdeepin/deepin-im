#include <wayland-server.h>

#include <filesystem>
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main()
{
    std::filesystem::path runtimeDir = getenv("XDG_RUNTIME_DIR");
    if (runtimeDir.empty()) {
        std::cerr << "XDG_RUNTIME_DIR is empty" << std::endl;
        return 1;
    }

    auto socketPath = runtimeDir / "imfakewl";
    sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, socketPath.c_str());

    int fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (fd == -1) {
        std::cerr << "failed to create socket:" << strerror(errno) << std::endl;
        return 1;
    }
    if (bind(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1) {
        std::cerr << "failed to bind:" << strerror(errno) << std::endl;
        return 1;
    }
    listen(fd, 100);

    struct wl_display *display = wl_display_create();
    if (!display) {
        std::cerr << "Unable to create Wayland display" << std::endl;
        return 1;
    }

    int ret = wl_display_add_socket_fd(display, fd);
    if (ret == -1) {
        std::cerr << "Unable to add socket to Wayland display" << std::endl;
        return 1;
    }

    std::cout << "Running Wayland display on " << socketPath << std::endl;
    wl_display_run(display);

    wl_display_destroy(display);

    close(fd);
    return 0;
}

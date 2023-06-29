#ifndef WAYLANDSERVER_H
#define WAYLANDSERVER_H

struct wl_display;

class WaylandServer {
public:
    WaylandServer();
    ~WaylandServer();

    void run();

private:
    wl_display *display_;
};

#endif // !WAYLANDSERVER_H

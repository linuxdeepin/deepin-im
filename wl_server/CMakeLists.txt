set(WL_SERVER_SOURCES
  main.cc
  WaylandServer.h
  WaylandServer.cc
)

add_executable(wl_server
  ${WL_SERVER_SOURCES}
)

target_link_libraries(wl_server PRIVATE
  PkgConfig::wayland-server
  Qt5::Core
  wayland_protocols
)

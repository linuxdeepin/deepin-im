set(WL_SERVER_SOURCES
  main.cc
  WaylandServer.h
  WaylandServer.cc
)

ecm_add_wayland_server_protocol(WL_SERVER_SOURCES
  PROTOCOL ${CMAKE_SOURCE_DIR}/misc/wayland-protocols/input-method-unstable-v1.xml
  BASENAME input-method-unstable-v1
)

ecm_add_wayland_server_protocol(WL_SERVER_SOURCES
  PROTOCOL ${CMAKE_SOURCE_DIR}/misc/wayland-protocols/text-input-unstable-v3.xml
  BASENAME text-input-unstable-v3
)

add_executable(wl_server
  ${WL_SERVER_SOURCES}
)

target_link_libraries(wl_server PRIVATE
  PkgConfig::wayland-server
)

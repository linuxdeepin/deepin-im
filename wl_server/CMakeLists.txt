set(WL_SERVER_SOURCES
  main.cc
)

add_executable(wl_server
  ${WL_SERVER_SOURCES}
)

target_link_libraries(wl_server PRIVATE
  PkgConfig::wayland-server
)

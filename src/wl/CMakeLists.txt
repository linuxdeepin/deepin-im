set(WL_SOURCES
  Connection.h
  Connection.cc
  Type.h
  Type.cc
)

add_library(wl STATIC
  ${WL_SOURCES}
)

target_link_libraries(wl PRIVATE
  Qt5::Core
  PkgConfig::wayland-client
  wayland_protocols
)

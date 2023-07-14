set(WLFRONTEND_SOURCES
  WLFrontend.h
  WLFrontend.cc
  WaylandInputContextV2.h
  WaylandInputContextV2.cc
)

add_library(wlfrontend SHARED
  ${WLFRONTEND_SOURCES}
)

target_link_libraries(wlfrontend PRIVATE
  dimcore
  Qt5::Core
  Qt5::DBus
  PkgConfig::wayland-client
  PkgConfig::xkbcommon
  wl
)

install(TARGETS wlfrontend
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES wlfrontend.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)

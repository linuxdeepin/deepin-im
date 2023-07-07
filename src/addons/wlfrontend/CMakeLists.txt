# configure_file(
#     config.h.in
#     config.h
# )

set(WLFRONTEND_SOURCES
  main.cc
  WaylandConnection.h
  WaylandConnection.cc
  WLFrontend.h
  WLFrontend.cc
  WaylandInputContextV2.h
  WaylandInputContextV2.cc
)

add_executable(wlfrontend
  ${WLFRONTEND_SOURCES}
)

target_link_libraries(wlfrontend PRIVATE
  dimcore
  Qt5::Core
  Qt5::DBus
  PkgConfig::wayland-client
  wayland_protocols
)

target_include_directories(wlfrontend PUBLIC
    ../../../
    utils/common.h
    )

install(TARGETS wlfrontend
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES wlfrontend.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)

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
  WaylandInputContextV1.h
  WaylandInputContextV1.cc
  InputContext1.h
  InputContext1.cc
)

add_executable(wlfrontend
  ${WLFRONTEND_SOURCES}
)

# add_library(wlfrontend MODULE
#   ${WLFRONTEND_SOURCES}
# )

ecm_add_wayland_client_protocol(wlfrontend
                                PROTOCOL ${CMAKE_SOURCE_DIR}/misc/wayland-protocols/input-method-unstable-v1.xml
                                BASENAME input-method-unstable-v1)

target_link_libraries(wlfrontend PRIVATE
  dimcore
  Qt5::Core
  Qt5::DBus
  PkgConfig::wayland-client
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

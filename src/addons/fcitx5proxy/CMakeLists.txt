set(FCITX5PROXY_SOURCES
  Fcitx5Proxy.h
  Fcitx5Proxy.cc
  DBusProvider.h
  DBusProvider.cc
)

add_library(fcitx5proxy MODULE
  ${FCITX5PROXY_SOURCES}
)

target_link_libraries(fcitx5proxy PRIVATE
  dimcore
  Qt5::Core
  Qt5::DBus
  Qt5::GuiPrivate
  Fcitx5Qt5::DBusAddons
)

target_include_directories(fcitx5proxy PUBLIC
    ../../../
    utils/common.h
    )

install(TARGETS fcitx5proxy
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES fcitx5proxy.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)


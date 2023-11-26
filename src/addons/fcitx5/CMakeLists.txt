set(FCITX5_SOURCES
  Fcitx5Proxy.h
  Fcitx5Proxy.cpp
  DBusProvider.h
  DBusProvider.cpp

  Listener.h
  Server.h
  Server.cpp
  Output.h
  Output.cpp
  View.h
  View.cpp
  Keyboard.h
  Keyboard.cpp
  InputMethodV2.h
  InputMethodV2.cpp
  InputPopupSurfaceV2.h
  InputPopupSurfaceV2.cpp
)

add_library(fcitx5 MODULE
  ${FCITX5_SOURCES}
)

include(${PROJECT_SOURCE_DIR}/cmake/WaylandScannerHelpers.cmake)

ws_generate(
    server
    wayland-protocols
    stable/xdg-shell/xdg-shell.xml
    xdg-shell-protocol
)

target_link_libraries(fcitx5 PRIVATE
  dimcore
  PkgConfig::wlroots
  PkgConfig::wayland-server
  Qt6::Core
  Qt6::DBus
  Qt6::GuiPrivate
  Fcitx5Qt6::DBusAddons
  $<$<BOOL:${Dtk6Core_FOUND}>:Dtk6::Core>
  wlc
)

install(TARGETS fcitx5
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES fcitx5.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)


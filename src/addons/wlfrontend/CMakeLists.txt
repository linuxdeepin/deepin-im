set(WLFRONTEND_SOURCES
  WLFrontend.h
  WLFrontend.cpp
  WlInputContext.h
  WlInputContext.cpp
  InputMethodV2.h
  InputMethodV2.cpp
  InputMethodKeyboardGrabV2.h
  InputMethodKeyboardGrabV2.cpp
  InputPopupSurfaceV2.h
  InputPopupSurfaceV2.cpp
)

add_library(wlfrontend SHARED
  ${WLFRONTEND_SOURCES}
)

target_link_libraries(wlfrontend PRIVATE
  dimcore
  Qt6::Core
  Qt6::DBus
  Qt6::GuiPrivate
  PkgConfig::wayland-client
  PkgConfig::xkbcommon
  wlc
)

install(TARGETS wlfrontend
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES wlfrontend.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)

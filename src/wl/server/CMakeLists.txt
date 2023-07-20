set(WLS_SOURCES
  Server.h
  Server.cc
  Global.h
  Global.cc
  Resource.h
  Resource.cc
  Seat.h
  Seat.cc
  ZwpTextInputManagerV3.h
  ZwpTextInputManagerV3.cc
  ZwpTextInputV3.h
  ZwpTextInputV3.cc
  ZwpInputMethodManagerV2.h
  ZwpInputMethodManagerV2.cc
  ZwpInputMethodV2.h
  ZwpInputMethodV2.cc
)

add_library(wls STATIC
  ${WLS_SOURCES}
)

target_link_libraries(wls PRIVATE
  Qt5::Core
  PkgConfig::wayland-server
  wayland_protocols
)

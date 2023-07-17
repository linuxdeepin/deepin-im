set(WL_SOURCES
  Connection.h
  Connection.cc
  Server.h
  Server.cc
  Type.h
  Type.cc
  Seat.h
  Seat.cc
  Global.h
  Global.cc
  ZwpInputMethodManagerV2.h
  ZwpInputMethodManagerV2.cc
  ZwpInputMethodV2.h
  ZwpInputMethodV2.cc
  ZwpVirtualKeyboardManagerV1.h
  ZwpVirtualKeyboardManagerV1.cc
  ZwpVirtualKeyboardV1.h
  ZwpVirtualKeyboardV1.cc
)

add_library(wl STATIC
  ${WL_SOURCES}
)

target_link_libraries(wl PRIVATE
  Qt5::Core
  PkgConfig::wayland-client
  wayland_protocols
)

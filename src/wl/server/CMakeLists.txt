set(WLS_SOURCES
  Server.h
  Server.cpp
  Global.h
  Global.cpp
  Resource.h
  Resource.cpp
  Seat.h
  Seat.cpp
  ZwpTextInputManagerV3.h
  ZwpTextInputManagerV3.cpp
  ZwpTextInputV3.h
  ZwpTextInputV3.cpp
  ZwpInputMethodManagerV2.h
  ZwpInputMethodManagerV2.cpp
  ZwpInputMethodV2.h
  ZwpInputMethodV2.cpp
  ZwpInputMethodKeyboardGrabV2.h
  ZwpInputMethodKeyboardGrabV2.cpp
  ZwpVirtualKeyboardManagerV1.h
  ZwpVirtualKeyboardManagerV1.cpp
  ZwpVirtualKeyboardV1.h
  ZwpVirtualKeyboardV1.cpp
)

add_library(wls STATIC
  ${WLS_SOURCES}
)

target_link_libraries(wls PRIVATE
  Qt6::Core
  PkgConfig::wayland-server
  wayland_protocols
)

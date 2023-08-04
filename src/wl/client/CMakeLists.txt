set(WLC_SOURCES
  ConnectionBase.h
  ConnectionBase.cpp
  ConnectionFd.h
  ConnectionFd.cpp
  Connection.h
  Connection.cpp
  Seat.h
  Seat.cpp
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
  ZwpTextInputManagerV3.h
  ZwpTextInputManagerV3.cpp
  ZwpTextInputV3.h
  ZwpTextInputV3.cpp
)

add_library(wlc STATIC)

target_sources(wlc
  PRIVATE ${WLC_SOURCES}
)

target_link_libraries(wlc PRIVATE
  Qt6::Core
  PkgConfig::wayland-client
  wayland_protocols
)

pkg_get_variable(XKEYBOARDCONFIG_XKBBASE xkeyboard-config xkb_base)
set(DEFAULT_XKB_RULES "evdev" CACHE STRING "Xkb rules name")

configure_file(
    config.h.in
    config.h
)

set(KEYBOARD_SOURCES
  keyboard.h
  keyboard.cc
)

add_library(keyboard MODULE
  ${KEYBOARD_SOURCES}
)

target_link_libraries(keyboard PRIVATE
  dimcore
  Qt5::Core
  Qt5::Xml
  PkgConfig::xkbcommon
)

install(TARGETS keyboard
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES keyboard.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)


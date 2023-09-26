cmake_minimum_required(VERSION 3.16)

project(imdemo VERSION 0.1 LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(ECM 1.4.0 REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Widgets Xml DBus WaylandCompositor)
find_package(Qt6Gui ${REQUIRED_QT6_VERSION} REQUIRED Private)
find_package(Fcitx5Qt6DBusAddons REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(WaylandScanner REQUIRED)
find_package(Dtk6Core QUIET)

if (Dtk6Core_FOUND)
  add_definitions(Dtk6Core_FOUND)
endif()

pkg_check_modules(xcb REQUIRED IMPORTED_TARGET xcb)
pkg_check_modules(xcb-xinput REQUIRED IMPORTED_TARGET xcb-xinput)
pkg_check_modules(xkbcommon REQUIRED IMPORTED_TARGET xkbcommon)
pkg_check_modules(xkeyboard-config REQUIRED IMPORTED_TARGET xkeyboard-config)
pkg_check_modules(wayland-client REQUIRED IMPORTED_TARGET wayland-client)
pkg_check_modules(wayland-server REQUIRED IMPORTED_TARGET wayland-server)
pkg_check_modules(libudev REQUIRED IMPORTED_TARGET libudev)
pkg_check_modules(libinput REQUIRED IMPORTED_TARGET libinput)

pkg_check_modules(Gtk3 REQUIRED IMPORTED_TARGET "gtk+-3.0")
pkg_check_modules(Gdk3 REQUIRED IMPORTED_TARGET "gdk-3.0")
pkg_get_variable(GTK3_BINARY_VERSION "gtk+-3.0" "gtk_binary_version")
pkg_get_variable(GTK3_TARGETS "gtk+-3.0" "targets")

add_library(wayland_protocols OBJECT)

ecm_add_wayland_server_protocol(wayland_protocols
  PROTOCOL misc/wayland-protocols/input-method-unstable-v2.xml
  BASENAME input-method-unstable-v2
)

ecm_add_wayland_server_protocol(wayland_protocols
  PROTOCOL misc/wayland-protocols/virtual-keyboard-unstable-v1.xml
  BASENAME virtual-keyboard-unstable-v1
)

ecm_add_wayland_server_protocol(wayland_protocols
  PROTOCOL misc/wayland-protocols/text-input-unstable-v3.xml
  BASENAME text-input-unstable-v3
)

ecm_add_wayland_server_protocol(wayland_protocols
  PROTOCOL misc/wayland-protocols/dim-text-input-unstable-v1.xml
  BASENAME dim-text-input-unstable-v1
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(misc)
add_subdirectory(demo)

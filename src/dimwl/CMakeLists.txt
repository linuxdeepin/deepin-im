add_definitions(-DWLR_USE_UNSTABLE)

add_executable(dimwl
    main.cpp
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
    TextInputV3.h
    TextInputV3.cpp
)


include(${PROJECT_SOURCE_DIR}/cmake/WaylandScannerHelpers.cmake)

ws_generate(
    server
    wayland-protocols
    stable/xdg-shell/xdg-shell.xml
    xdg-shell-protocol
)

target_link_libraries(dimwl PUBLIC
    PkgConfig::wlroots
    PkgConfig::wayland-server
    PkgConfig::xkbcommon
)

install(TARGETS dimwl
    BUNDLE BUNDLE DESTINATION .
)

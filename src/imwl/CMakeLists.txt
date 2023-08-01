qt_add_executable(imwl
    main.cpp
    common.cpp common.h
    Core.h
    Compositor.cpp Compositor.h
    TextInputManagerV3.cpp TextInputManagerV3.h
    TextInputV3.cpp TextInputV3.h
    InputMethodManagerV2.cpp InputMethodManagerV2.h
    InputMethodV2.cpp InputMethodV2.h
    InputMethodKeyboardGrabV2.cpp InputMethodKeyboardGrabV2.h
    VirtualKeyboardManagerV1.cpp VirtualKeyboardManagerV1.h
    VirtualKeyboardV1.cpp VirtualKeyboardV1.h
    X11KeyboardGrabber.cpp X11KeyboardGrabber.h
)

qt6_generate_wayland_protocol_server_sources(imwl
    FILES
        ${CMAKE_SOURCE_DIR}/misc/wayland-protocols/text-input-unstable-v3.xml
        ${CMAKE_SOURCE_DIR}/misc/wayland-protocols/input-method-unstable-v2.xml
        ${CMAKE_SOURCE_DIR}/misc/wayland-protocols/virtual-keyboard-unstable-v1.xml
)

target_link_libraries(imwl PUBLIC
    Qt::Core
    Qt::Gui
    Qt::WaylandCompositor
    PkgConfig::xcb
    PkgConfig::xcb-xinput
    PkgConfig::xkbcommon
)

install(TARGETS imwl
    BUNDLE BUNDLE DESTINATION .
)

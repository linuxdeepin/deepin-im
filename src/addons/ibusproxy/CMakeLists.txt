set(CMAKE_C_STANDARD 99)

set(IBUSPROXY_SOURCES
    IBusProxy.h
    IBusProxy.cc
)

qt5_add_dbus_interface(IC_SOURCES
    ${CMAKE_SOURCE_DIR}/misc/ibus-inputcontext.xml
    IBUSInputContextIface
)

add_library(ibusproxy MODULE
    ${IBUSPROXY_SOURCES}
    ${IC_SOURCES}
)

target_compile_definitions(ibusproxy PRIVATE -DQT_NO_KEYWORDS)

target_link_libraries(ibusproxy PRIVATE
    GLIB2::GLIB2
    PkgConfig::IBUS
    PkgConfig::GOBJECT
    Qt::Core
    Qt::DBus
)

install(TARGETS ibusproxy
    LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES ibusproxy.conf
    DESTINATION ${DIM_ADDON_INFO_DIR}
)



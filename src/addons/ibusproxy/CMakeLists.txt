set(CMAKE_C_STANDARD 99)

set(IBUSPROXY_SOURCES
    IBusProxy.h
    IBusProxy.cpp
    ibustypes.h
    ibustypes.cpp
)

qt_add_dbus_interface(IC_SOURCES
    ${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.IBus.InputContext.xml
    IBUSInputContextIface
)

qt_add_dbus_interface(IC_SOURCES
    ${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.IBus.Portal.xml
    IBUSPortIface
)

set_source_files_properties(${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.IBus.xml
    PROPERTIES NO_NAMESPACE ON
)

qt_add_dbus_interface(IC_SOURCES
    ${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.IBus.xml
    IBUSIface
)

add_library(ibusproxy MODULE
    ${IBUSPROXY_SOURCES}
    ${IC_SOURCES}
)

target_compile_definitions(ibusproxy PRIVATE -DQT_NO_KEYWORDS)

target_link_libraries(ibusproxy PRIVATE
    dimcore
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::GuiPrivate
)

install(TARGETS ibusproxy
    LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES ibusproxy.conf
    DESTINATION ${DIM_ADDON_INFO_DIR}
)



set(KIMPANEL_SOURCES
  main.cc
  Panel.h
  Panel.cc
  qml.qrc
)

qt5_add_dbus_adaptor(KIMPANEL_SOURCES org.kde.impanel.xml Panel.h Panel ImpanelAdaptor)
qt5_add_dbus_adaptor(KIMPANEL_SOURCES org.kde.impanel2.xml Panel.h Panel Impanel2Adaptor)
qt5_add_dbus_interface(KIMPANEL_SOURCES org.kde.impanel.inputmethod.xml ImpanelInputmethod)

add_executable(kimpanel
  ${KIMPANEL_SOURCES}
)

target_link_libraries(kimpanel PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::DBus
)

install(TARGETS kimpanel
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

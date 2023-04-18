set(IMDEMO_SOURCES
  main.cc
  KIMPanel.h
  KIMPanel.cc
  KIMPanelAdaptor.h
  KIMPanelAdaptor.cc
  KIMPanel2Adaptor.h
  KIMPanel2Adaptor.cc
  qml.qrc
)

qt5_add_dbus_interface(IMDEMO_SOURCES org.kde.impanel.inputmethod.xml KimpanelInputmethod)

add_executable(kimpanel
  ${IMDEMO_SOURCES}
)

target_link_libraries(kimpanel PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::DBus
)

install(TARGETS kimpanel
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

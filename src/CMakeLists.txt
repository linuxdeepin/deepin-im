set(IMDEMO_SOURCES
  main.cc
  KIMPanel/KIMPanel.h
  KIMPanel/KIMPanel.cc
  KIMPanel/KIMPanelAdaptor.h
  KIMPanel/KIMPanelAdaptor.cc
  KIMPanel/KIMPanel2Adaptor.h
  KIMPanel/KIMPanel2Adaptor.cc
  qml.qrc
)

qt5_add_dbus_interface(IMDEMO_SOURCES KIMPanel/org.kde.impanel.inputmethod.xml KimpanelInputmethod)

add_executable(untitled
  ${IMDEMO_SOURCES}
)

target_link_libraries(untitled PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::DBus
)

install(TARGETS untitled
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(QT_TEST_DEMO_SOURCES
  main.cpp
  qml.qrc
)

add_executable(qt_im_window
  ${QT_TEST_DEMO_SOURCES}
)

target_link_libraries(qt_im_window PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::DBus
)

install(TARGETS qt_im_window
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

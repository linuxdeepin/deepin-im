set(DEMOWINDOW_SOURCES
  main.cpp
  qml.qrc
)

add_executable(demowindow
  ${DEMOWINDOW_SOURCES}
)

target_link_libraries(demowindow PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::DBus
)

install(TARGETS demowindow
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set(IMDEMOTEST_SOURCES
  main.cpp
  MainWindow.h
  MainWindow.cpp
)

add_executable(imdemotest
  ${IMDEMOTEST_SOURCES}
)

target_link_libraries(imdemotest PRIVATE
  Qt6::Core
  Qt6::Widgets
  Qt6::DBus
)

install(TARGETS imdemotest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


set(DIM_SOURCES
  main.cc
  Dim.h
  Dim.cc
  InputContext.h
  InputContext.cc
)

qt5_add_dbus_adaptor(DIM_SOURCES org.deepin.dde.inputmethod.xml Dim.h Dim InputMethodAdaptor)
qt5_add_dbus_adaptor(DIM_SOURCES org.deepin.dde.inputcontext.xml InputContext.h InputContext InputContextAdaptor)

add_executable(dim
  ${DIM_SOURCES}
)

target_link_libraries(dim PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::DBus
)

install(TARGETS dim
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

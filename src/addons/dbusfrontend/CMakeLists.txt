# configure_file(
#     config.h.in
#     config.h
# )

set(DBUSFRONTEND_SOURCES
  DBusFrontend.h
  DBusFrontend.cc
  InputContext1.h
  InputContext1.cc
)

qt5_add_dbus_adaptor(DBUSFRONTEND_SOURCES ${CMAKE_SOURCE_DIR}/misc/inputmethod.xml DBusFrontend.h DBusFrontend InputmethodAdaptor)
qt5_add_dbus_adaptor(DBUSFRONTEND_SOURCES ${CMAKE_SOURCE_DIR}/misc/inputcontext.xml InputContext1.h InputContext1 InputcontextAdaptor)

add_library(dbusfrontend MODULE
  ${DBUSFRONTEND_SOURCES}
)

target_link_libraries(dbusfrontend PRIVATE
  dimcore
  Qt5::Core
  Qt5::DBus
)

install(TARGETS dbusfrontend
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES dbusfrontend.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)

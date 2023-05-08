set(DIM_DATA_DIR ${CMAKE_INSTALL_FULL_DATAROOTDIR}/dim)
set(DIM_ADDON_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/dim)

configure_file(
  config.h.in
  config.h
)

set(DIMCORE_SOURCES
  Dim.cc
  Addon.cc
  InputContext.cc
  # InputMethodAddon.cc
)

set(DIMCORE_HEADERS
  Dim.h
  Addon.h
  InputContext.h
  InputMethodAddon.h
  InputMethodEntry.h
)

add_library(dimcore SHARED
  ${DIMCORE_SOURCES}
  ${DIMCORE_HEADERS}
)

target_include_directories(dimcore PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(dimcore PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::DBus
  ${CMAKE_DL_LIBS}
)

install(TARGETS dimcore
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

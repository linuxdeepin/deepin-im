set(DIMCORE_SOURCES
  Dim.cc
  Addon.cc
  # InputMethod.cc
  # InputMethodAddon.cc
)

set(DIMCORE_HEADERS
  Dim.h
  Addon.h
  InputMethodAddon.h
  InputMethodEntry.h
)

add_library(dimcore SHARED
  ${DIMCORE_SOURCES}
  ${DIMCORE_HEADERS}
)

target_link_libraries(dimcore PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::DBus
)

install(TARGETS dimcore
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

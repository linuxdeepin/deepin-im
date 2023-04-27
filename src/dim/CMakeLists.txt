set(DIM_SOURCES
  main.cc
)

add_executable(dim
  ${DIM_SOURCES}
)

target_link_libraries(dim PRIVATE
  dimcore
  Qt5::Core
)

install(TARGETS dim
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

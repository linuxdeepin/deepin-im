set(DIM_SOURCES
  main.cpp
)

add_executable(dim
  ${DIM_SOURCES}
)

target_link_libraries(dim PRIVATE
  dimcore
  Qt6::Core
)

install(TARGETS dim
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

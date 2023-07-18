set(WLSERVER_SOURCES
  main.cc
)

add_library(wlserver STATIC
  ${WLSERVER_SOURCES}
)

target_link_libraries(wlserver PRIVATE
  wls
)

install(TARGETS wlserver
  BUNDLE DESTINATION .
)

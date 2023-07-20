set(WLSERVER_SOURCES
  main.cc
  WLServer.h
  WLServer.cc
)

add_executable(wlserver
  ${WLSERVER_SOURCES}
)

target_link_libraries(wlserver PRIVATE
  wls
  PkgConfig::libudev
  PkgConfig::libinput
)

install(TARGETS wlserver
  BUNDLE DESTINATION .
)

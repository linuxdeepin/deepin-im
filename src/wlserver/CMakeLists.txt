set(WLSERVER_SOURCES
  main.cpp
  WLServer.h
  WLServer.cpp
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

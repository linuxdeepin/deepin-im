set(QTFRONTEND_SOURCES
  DIMPlatformInputContextPlugin.h
  DIMPlatformInputContextPlugin.cc
  DIMPlatformInputContext.h
  DIMPlatformInputContext.cc
  InputContextProxy.h
  InputContextProxy.cc
)

add_library(dimplatforminputcontextplugin MODULE
  ${QTFRONTEND_SOURCES}
)

target_include_directories(dimplatforminputcontextplugin  PRIVATE
)

target_link_libraries(dimplatforminputcontextplugin PRIVATE
  PkgConfig::wayland-client
  Qt6::GuiPrivate
  Qt6::DBus
  wlc
)

include(ECMQueryQt)
ecm_query_qt(_QTPLUGINDIR QT_INSTALL_PLUGINS)
set(CMAKE_INSTALL_QTPLUGINDIR ${_QTPLUGINDIR} CACHE PATH "Qt plugin dir")
install(TARGETS dimplatforminputcontextplugin
  DESTINATION ${CMAKE_INSTALL_QTPLUGINDIR}/platforminputcontexts
)

set(QTIMMODULE_SOURCES
  DIMPlatformInputContextPlugin.h
  DIMPlatformInputContextPlugin.cpp
  DIMPlatformInputContext.h
  DIMPlatformInputContext.cpp
  DimTextInputV1.h
  DimTextInputV1.cpp

  QWaylandTextInputInterface.h
  QWaylandInputMethodEventBuilder.h
  QWaylandInputMethodEventBuilder.cpp
)

add_library(dimplatforminputcontextplugin MODULE
  ${QTIMMODULE_SOURCES}
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

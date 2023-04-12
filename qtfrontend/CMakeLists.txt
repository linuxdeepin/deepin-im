set(QTFRONTEND_SOURCES
  DIMPlatformInputContextPlugin.h
  DIMPlatformInputContextPlugin.cc
  DIMPlatformInputContext.h
  DIMPlatformInputContext.cc
)

add_library(dimplatforminputcontextplugin MODULE
  ${QTFRONTEND_SOURCES}
)

target_include_directories(dimplatforminputcontextplugin  PRIVATE
)

target_link_libraries(dimplatforminputcontextplugin PRIVATE
  Qt5::GuiPrivate
)

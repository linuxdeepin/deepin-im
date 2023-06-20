set(QTFRONTEND_SOURCES
  DIMPlatformInputContextPlugin.h
  DIMPlatformInputContextPlugin.cc
  DIMPlatformInputContext.h
  DIMPlatformInputContext.cc
  InputContextProxy.h
  InputContextProxy.cc
)

qt5_add_dbus_interface(QTFRONTEND_SOURCES ${CMAKE_SOURCE_DIR}/misc/inputmethod.xml InputmethodIface)
qt5_add_dbus_interface(QTFRONTEND_SOURCES ${CMAKE_SOURCE_DIR}/misc/inputcontext.xml InputcontextIface)

add_library(dimplatforminputcontextplugin MODULE
  ${QTFRONTEND_SOURCES}
)

target_include_directories(dimplatforminputcontextplugin  PRIVATE
)

target_link_libraries(dimplatforminputcontextplugin PRIVATE
  Qt5::GuiPrivate
  Qt5::DBus
)

target_include_directories(dimplatforminputcontextplugin PUBLIC
    ../
    utils/common.h
    )

include(ECMQueryQmake)
query_qmake(_QT5PLUGINDIR QT_INSTALL_PLUGINS)
set(CMAKE_INSTALL_QT5PLUGINDIR ${_QT5PLUGINDIR} CACHE PATH "Qt5 plugin dir")
install(TARGETS dimplatforminputcontextplugin
  DESTINATION ${CMAKE_INSTALL_QT5PLUGINDIR}/platforminputcontexts
)

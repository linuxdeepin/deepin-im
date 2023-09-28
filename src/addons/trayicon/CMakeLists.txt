set(TRAYICON_SOURCES
  TrayIcon.h
  TrayIcon.cpp
  IconProvider.h
  IconProvider.cpp
  qml.qrc
)

add_library(trayicon SHARED
  ${TRAYICON_SOURCES}
)

target_link_libraries(trayicon PRIVATE
  dimcore
  Qt6::Core
  Qt6::Quick
  Qt6::Widgets
  $<$<BOOL:${Dtk6Core_FOUND}>:Dtk6::Core>
)

install(TARGETS trayicon
  LIBRARY DESTINATION ${DIM_ADDON_DIR}
)

install(FILES trayicon.conf
  DESTINATION ${DIM_ADDON_INFO_DIR}
)

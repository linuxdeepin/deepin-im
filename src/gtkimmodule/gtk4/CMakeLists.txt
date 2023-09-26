set(IM_DIM_GTK4_SOURCES
  main.cpp
  imcontext.h
  imcontext.cpp
  DimGtkTextInputV1.h
  DimGtkTextInputV1.cpp
)

add_library(im-dim-gtk4 MODULE
  ${IM_DIM_GTK4_SOURCES}
)

set_target_properties(im-dim-gtk4 PROPERTIES OUTPUT_NAME "im-dim"
  COMPILE_FLAGS "-fno-exceptions")

if (NOT DEFINED GTK4_IM_MODULEDIR)
  set(GTK4_IM_MODULEDIR "${CMAKE_INSTALL_LIBDIR}/gtk-4.0/${GTK4_BINARY_VERSION}/immodules" CACHE PATH "Gtk4 im module directory")
endif()

target_link_libraries(im-dim-gtk4
  PkgConfig::Gtk4
  wlc
)

install(TARGETS im-dim-gtk4 DESTINATION "${GTK4_IM_MODULEDIR}")
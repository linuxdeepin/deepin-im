set(IM_DIM_GTK3_SOURCES
  main.cpp
  imcontext.h
  imcontext.cpp
)

add_library(im-dim-gtk3 MODULE
  ${IM_DIM_GTK3_SOURCES}
)

set(GTK3_IM_MODULEDIR "${CMAKE_INSTALL_LIBDIR}/gtk-3.0/${GTK3_BINARY_VERSION}/immodules" CACHE PATH "Gtk3 im module directory")

set_target_properties(im-dim-gtk3 PROPERTIES PREFIX "" OUTPUT_NAME "im-dim")

target_link_libraries(im-dim-gtk3
  PkgConfig::Gtk3
  PkgConfig::graphene
)

install(TARGETS im-dim-gtk3 DESTINATION "${GTK3_IM_MODULEDIR}")
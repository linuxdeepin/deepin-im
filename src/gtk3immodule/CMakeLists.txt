set(GTK3_IM_MODULEDIR "${CMAKE_INSTALL_LIBDIR}/gtk-3.0/${GTK3_BINARY_VERSION}/immodules" CACHE PATH "Gtk3 im module directory")

add_library(im-dim-gtk3)
set_target_properties(im-dim-gtk3 PROPERTIES PREFIX "" OUTPUT_NAME "im-dim")

target_sources(im-dim-gtk3 PRIVATE
  main.cpp
  imcontext.h
  imcontext.cpp
)

target_link_libraries(im-dim-gtk3
  PkgConfig::Gtk3
)

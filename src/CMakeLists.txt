include_directories(.)

set(DIM_DATA_DIR ${CMAKE_INSTALL_DATAROOTDIR}/dim)
set(DIM_ADDON_INFO_DIR ${DIM_DATA_DIR}/addon)
set(DIM_ADDON_DIR ${CMAKE_INSTALL_LIBDIR}/dim)

add_subdirectory(wl)
add_subdirectory(wlserver)
add_subdirectory(dimcore)
add_subdirectory(dim)
add_subdirectory(addons)
add_subdirectory(kimpanel)
add_subdirectory(qtfrontend)

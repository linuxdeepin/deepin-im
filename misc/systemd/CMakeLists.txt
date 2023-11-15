if (NOT DEFINED SYSTEMD_USER_UNIT_DIR)
    pkg_get_variable(SYSTEMD_USER_UNIT_DIR systemd systemduserunitdir)
endif()

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/org.deepin.dde.dim.service.in
    org.deepin.dde.dim.service
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/org.deepin.dde.dimwl.service.in
    org.deepin.dde.dimwl.service
    @ONLY
)

set(SERVICES
    ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.dde.dimwl.service
    ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.dde.dim.service
)

install(FILES ${SERVICES} DESTINATION ${SYSTEMD_USER_UNIT_DIR})

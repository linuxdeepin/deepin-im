find_package(PkgConfig REQUIRED)
find_package(DdeControlCenter REQUIRED)
find_package(DtkCore REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(
  Qt5
  COMPONENTS Widgets DBus Test
  REQUIRED)
pkg_check_modules(xkbcommon REQUIRED IMPORTED_TARGET xkbcommon)

set(BIN_NAME dcc-im-plugin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

file(
  GLOB_RECURSE
  HEAD_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/window/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/window/widgets/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/operation/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imconfigplugin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h)
file(
  GLOB_RECURSE
  SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/window/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/widgets/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/operation/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imconfigplugin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp)

set(APP_QRC "${CMAKE_CURRENT_SOURCE_DIR}/res/imconfig.qrc")
set(ALL_SRCS ${HEAD_FILES} ${SOURCE_FILES} ${APP_QRC})

add_compile_definitions(IM_GETTEXT_DOMAIN="im-configtool")
add_definitions("-DFCITX_GETTEXT_DOMAIN=\"fcitx5\"")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  message("Debug----------------")
  add_executable(${BIN_NAME} ${ALL_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
endif()

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_library(${BIN_NAME} SHARED ${ALL_SRCS})
endif()

target_include_directories(
  ${BIN_NAME} PRIVATE ${DdeControlCenter_INCLUDE_DIR} ${DtkCore_INCLUDE_DIRS}
                      ${DtkWidget_INCLUDE_DIRS})

target_link_libraries(
  ${BIN_NAME}
  PRIVATE dcc-interface
          dcc-widgets
          Qt5::Widgets
          Qt5::DBus
          ${DtkCore_LIBRARIES}
          ${DtkWidget_LIBRARIES}
          PkgConfig::xkbcommon)

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
  install(
    TARGETS ${BIN_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/dde-control-center/modules)
endif()

# 调用外部程序  生成qm文件
execute_process(COMMAND bash translate_generation.sh
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# qm files
file(GLOB QM_FILES "translations/*.qm")
install(
  FILES ${QM_FILES}
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dde-control-center/translations)

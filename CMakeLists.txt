cmake_minimum_required(VERSION 3.14)

project(imdemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Widgets DBus)
find_package(Qt5Gui ${REQUIRED_QT5_VERSION} REQUIRED Private)

add_subdirectory(kimpanel)
add_subdirectory(qtfrontend)

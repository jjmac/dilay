cmake_minimum_required (VERSION 3.0)

project (lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)

add_definitions(-DDILAY_VERSION="1")
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
include_directories(${PROJECT_SOURCE_DIR}/src ${glm_INCLUDE_DIRS})

file(GLOB_RECURSE CPP  ${PROJECT_SOURCE_DIR}/src *.cpp)
file(GLOB_RECURSE H  ${PROJECT_SOURCE_DIR}/src *.h)
file(GLOB_RECURSE HPP  ${PROJECT_SOURCE_DIR}/src *.hpp)
add_library(lib ${CPP}  ${H} ${HPP})
target_link_libraries (lib glm)
target_link_libraries(lib Qt5::Core Qt5::Widgets Qt5::Xml)

set(lib_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "lib: Include Directories" FORCE)

cmake_minimum_required (VERSION 3.0)

project (app)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DDILAY_VERSION="1")


file(GLOB_RECURSE CPP  ${PROJECT_SOURCE_DIR}/src *.cpp)
file(GLOB_RECURSE H  ${PROJECT_SOURCE_DIR}/src *.h)
file(GLOB_RECURSE HPP  ${PROJECT_SOURCE_DIR}/src *.hpp)

include_directories(${PROJECT_SOURCE_DIR}/src ${lib_INCLUDE_DIRS})

add_executable(app ${CPP} ${H} ${HPP})
target_link_libraries (app lib)

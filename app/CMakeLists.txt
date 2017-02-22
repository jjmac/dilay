cmake_minimum_required (VERSION 3.0)

project (app)

file(GLOB_RECURSE CPP  ${PROJECT_SOURCE_DIR}/src *.cpp)
file(GLOB_RECURSE H  ${PROJECT_SOURCE_DIR}/src *.h)
file(GLOB_RECURSE HPP  ${PROJECT_SOURCE_DIR}/src *.hpp)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
include_directories(${PROJECT_SOURCE_DIR}/src ${lib_INCLUDE_DIRS})

add_executable(app ${CPP} ${H} ${HPP})
target_link_libraries (app lib)

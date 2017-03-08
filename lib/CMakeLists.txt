cmake_minimum_required (VERSION 3.0)

project (lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)

add_definitions(-DDILAY_VERSION="1")
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
include_directories(${PROJECT_SOURCE_DIR}/src ${glm_INCLUDE_DIRS})


function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()	
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

file(GLOB_RECURSE SOURCE_LIST  ${PROJECT_SOURCE_DIR}/src "*.cpp" "*.hpp" "*.h")
assign_source_group(${SOURCE_LIST} )

add_library(lib ${SOURCE_LIST})
target_link_libraries (lib glm)
target_link_libraries(lib Qt5::Core Qt5::Widgets Qt5::Xml)

set(lib_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "lib: Include Directories" FORCE)

	

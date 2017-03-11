cmake_minimum_required (VERSION 3.0)

project (ext)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(glm)
add_subdirectory(json)

set(glm_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/glm
    CACHE INTERNAL "glm: Include Directories" FORCE)

set(json_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/json/src
    CACHE INTERNAL "json: Include Directories" FORCE)

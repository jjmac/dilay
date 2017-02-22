cmake_minimum_required (VERSION 3.0)

project (ext)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(glm)

set(glm_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/glm
    CACHE INTERNAL "glm: Include Directories" FORCE)
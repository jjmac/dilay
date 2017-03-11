cmake_minimum_required (VERSION 3.0)

project (nlohman_json VERSION 2.1.1 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCE_LIST  ${PROJECT_SOURCE_DIR}/src "*.cpp" "*.hpp" "*.h")
include_directories(${PROJECT_SOURCE_DIR}/src    )

add_library(nlohmann_json INTERFACE)
set(JSON_INCLUDE_DESTINATION "include/nlohmann")
install(FILES ${SOURCE_LIST} DESTINATION ${JSON_INCLUDE_DESTINATION})


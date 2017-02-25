cmake_minimum_required (VERSION 3.0)

project (qmlapp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Qml)
find_package(Qt5Quick)

file(GLOB_RECURSE CPP  ${PROJECT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE H  ${PROJECT_SOURCE_DIR} *.h)
file(GLOB_RECURSE HPP  ${PROJECT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE QML  ${PROJECT_SOURCE_DIR} *.qml)
 
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
include_directories(${PROJECT_SOURCE_DIR}/src ${lib_INCLUDE_DIRS})

qt5_add_resources(UI_RESOURCES qml.qrc)
add_executable(qmlapp ${CPP} ${H} ${HPP} ${UI_RESOURCES} ${QML})
target_link_libraries (qmlapp lib)
target_link_libraries (qmlapp Qt5::Qml Qt5::Quick)

cmake_minimum_required(VERSION 2.8)

project(Cloth)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -DPROG_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Atlas REQUIRED)
include_directories(
        "${ATLAS_INCLUDE_DIR}"
        "${CMAKE_SOURCE_DIR}/inc"
        "${CMAKE_SOURCE_DIR}/generated"
        )

add_subdirectory("${CMAKE_SOURCE_DIR}/inc")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/shaders")


add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_INCLUDE_LIST} ${PROJECT_SOURCE_LIST})
target_link_libraries(${CMAKE_PROJECT_NAME} ${ATLAS_LIBRARY} GL glfw GLEW)

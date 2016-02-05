set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/ShaderPaths.hpp.in"
        "${CMAKE_SOURCE_DIR}/generated/ShaderPaths.hpp")


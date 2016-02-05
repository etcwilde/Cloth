# Try to fing GLM
# Defines:
#
# GLM_FOUND
# GLM_INCLUDE_DIR
#

find_path(GLM_INCLUDE_DIR
        NAMES
                glm/glm.hpp
        HINTS
                "${GLM_LOCATION}/include"
                "$ENV{GLM_LOCATION}/include"
        PATHS
                /usr/include
                /usr/local/include
                /opt/graphics/OpenGL/include
        DOC
                "Directory where glm/glm.hpp resides"
)

set(GLM_FOUND "NO")
if(GLM_INCLUDE_DIR)
        set(GLM_FOUND "YES")
endif(GLM_INCLUDE_DIR)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM REQUIRED_VARS GLM_INCLUDE_DIR)
mark_as_advanced(GLM_INCLUDE_DIR)

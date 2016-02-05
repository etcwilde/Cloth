# Try to find GLI
# Defines:
#
# GLI_FOUND
# GLI_INCLUDE_DIR
#


find_path(GLI_INCLUDE_DIR
        NAMES
                gli/gli.hpp
        HINTS
                "${GLI_LOCATION}/include"
                "$ENV{GLI_LOCATION}/include"
        PATHS
                "$ENV{PROGRAMFILES}/GLI/include"
                "${OPENGL_INCLUDE_DIR}"
                /usr/openwin/share/include
                /usr/openwin/include
                /usr/X11R6/include
                /usr/include/X11
                /opt/graphics/OpenGL/include
                /opt/graphics/OpenGL/contrib/libgli
                /usr/local/include
                /usr/include/GL
                /usr/include
        DOC
                "The directory where gli/gli.hpp resides"
)

# if (NOT GLI_INCLUDE_DIR)
#       MESSAGE(FATAL_ERROR "GLI NOT FOUND")
# endif()

set(GLI_FOUND "NO")

if(GLI_INCLUDE_DIR)
        # Attempt to find the GLI_VERSION numbers from the library headers
        function(parseVersion FILENAME VARNAME)
                set(PATTERN "^#define ${VARNAME}.*$")
                file(STRINGS "${GLI_INCLUDE_DIR}/${FILENAME}" TMP REGEX ${PATTERN})
                string(REGEX MATCHALL "[0-9]+" TMP ${TMP})
                set(${VARNAME} ${TMP} PARENT_SCOPE)
        endfunction()
        if(EXISTS "${GLI_INCLUDE_DIR}/gli/gli.hpp")
                parseVersion("gli/gli.hpp" GLI_VERSION_MAJOR)
                parseVersion("gli/gli.hpp" GLI_VERSION_MINOR)
                parseVersion("gli/gli.hpp" GLI_VERSION_PATCH)
        endif()
        if(${GLI_VERSION_MAJOR} OR ${GLI_VERSION_MINOR} OR ${GLI_VERSION_PATCH})
                set(GLI_VERSION
                        "${GLI_VERSION_MAJOR}.${GLI_VERSION_MINOR}.${GLI_VERSION_PATCH}")
                set(GLI_VERSION_STRING "${GLI_VERSION}")
                mark_as_advanced(GLI_VERSION)
        endif()
        set(GLI_FOUND "YES")
endif(GLI_INCLUDE_DIR)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLI
        REQUIRED_VARS
                GLI_INCLUDE_DIR
        VERSION_VAR
                GLI_VERSION
                )
mark_as_advanced(GLI_INCLUDE_DIR)

#
# 2016 Evan Wilde <etcwilde@uvic.ca>
#

# Finds the Atlas Library and include path
# Once done, this will define:
#
# ATLAS_FOUND
# ATLAS_INCLUDE_DIR
# ATLAS_LIBRARY

include(FindPackageHandleStandardArgs)

find_path(ATLAS_INCLUDE_DIR
        NAMES
                atlas/Atlas.hpp
        HINTS
                "${ATLAS_LOCATION}/include"
                "$ENV{ATLAS_LOCATION}/include"
        PATHS
                "$ENV{PROGRAMFILES}/include"
                "${ATLAS_INCLUDE_DIR}"
                /usr/openwin/share/include
                /usr/openwin/include
                /usr/local/include
                /usr/include
        DOC
                "The directory where atlas/Atlas.hpp resides"
)

if (WIN32)
        if(CYGWIN)
                find_library(ATLAS_LIBRARY
                        NAMES
                                Atlas
                                atlas
                        HINTS
                                "${ALTAS_LOCATION}/lib"
                                "${ATLAS_LOCATION}/lib/x64"
                                "$ENV{ATLAS_LOCATION}/lib"
                        PATHS
                                "${ATLAS_LIBRARY_DIR}'"
                                /usr/lib64
                                /usr/lib
                                /usr/local/lib64
                                /usr/local/lib
                        DOC
                                "The Atlas library"
                        )
        else()
                find_library(ATLAS_LIBRARY
                        NAMES
                                Atlas
                                atlas
                        HINTS
                                "${ATLAS_LOCATION}/lib"
                                "${ATLAS_LOCATION}/lib/x64"
                                "${ATLAS_LOCATION}/lib-msvc110"
                                "${ATLAS_LOCATION}/lib-vc2012"
                                "$ENV{ATLAS_LOCATION}/lib"
                                "$ENV{ATLAS_LOCATION}/lib/x64"
                                "$ENV{ATLAS_LOCATION}/lib-msvc110"
                                "$ENV{ATLAS_LOCATION}/lib-vc2012"
                        PATHS
                                "$ENV{PROGRAMFILES}/atlas/lib"
                                "${ATLAS_LOCATION}"
                        DOC
                                "The Atlas library"
                )
        endif()
else()
        if(APPLE)
                find_library(ATLAS_LIBRARY Atlas atlas
                        NAMES
                                Atlas
                        HINTS
                                "${ATLAS_LOCATION}/lib"
                                "${ATLAS_LOCATION}/lib/cocoa"
                                "$ENV{ATLAS_LOCATION}/lib"
                                "$ENV{ATLAS_LOCATION}/lib/cocoa"
                        PATHS
                                /usr/local/lib
                        DOC
                                "The Atlas library"
                        )
        else()
                find_package(GLFW REQUIRED)
                find_package(GLEW REQUIRED)

                find_library(ATLAS_LIBRARY
                        NAMES
                                atlas
                                Atlas
                        HINTS
                                "${ATLAS_LOCATION}/lib"
                                "$ENV{ATLAS_LOCATION}/lib"
                        PATHS
                                /usr/lib64
                                /usr/lib
                                /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
                                /usr/local/lib64
                                /usr/local/lib
                                /usr/local/lib/${CMAKE_LIBRARY_ARCHITECTURE}
                        DOC
                                "The Atlas library"
                        )
        endif()
endif()

set(ATLAS_FOUND "NO")
if(ATLAS_INCLUDE_DIR)
        if(ATLAS_LIBRARY)
                set(ATLAS_FOUND "YES")
                set(ATLAS_INCLUDE_PATH "${ATLAS_INCLUDE_DIR}")
        endif(ATLAS_LIBRARY)
endif(ATLAS_INCLUDE_DIR)

find_package_handle_standard_args(ATLAS
        REQUIRED_VARS
                ATLAS_LIBRARY
                ATLAS_INCLUDE_DIR
                )

mark_as_advanced(
        ATLAS_INCLUDE_DIR
        ATLAS_LIBRARY
)

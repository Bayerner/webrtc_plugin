#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the GoCastPlayer project
#
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

Project(${PLUGIN_NAME})

file (GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    )

include_directories(${PLUGIN_INCLUDE_DIRS})

include_directories(../../../../deps/libjingle/trunk/third_party/webrtc)
include_directories(../../../../deps/libjingle/trunk/third_party/libyuv/include)
include_directories(../../../../deps/libjingle/trunk)

add_definitions(-DWEBRTC_RELATIVE_PATH)
add_definitions(-DGTEST_RELATIVE_PATH)

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
        GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${GENERATED}
    )

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()

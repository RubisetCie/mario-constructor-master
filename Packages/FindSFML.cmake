# Define the list of search paths for headers and libraries
set(FIND_SFML_PATHS
    "${SFML_ROOT}"
    "$ENV{SFML_ROOT}"
    "/usr/local"
    "/usr"
    "/sw"
    "/opt/local"
    "/opt/csw"
    "/opt"
)

# Find the SFML include directory
find_path(SFML_INCLUDE_DIR SFML/Config.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_SFML_PATHS})

# Check the version number
set(SFML_VERSION_OK TRUE)
if(SFML_FIND_VERSION AND SFML_INCLUDE_DIR)
    # Extract the major and minor version numbers from SFML/Config.hpp
    # We have to handle framework a little bit differently:
    if("${SFML_INCLUDE_DIR}" MATCHES "SFML.framework")
        set(SFML_CONFIG_HPP_INPUT "${SFML_INCLUDE_DIR}/Headers/Config.hpp")
    else()
        set(SFML_CONFIG_HPP_INPUT "${SFML_INCLUDE_DIR}/SFML/Config.hpp")
    endif()
    FILE(READ "${SFML_CONFIG_HPP_INPUT}" SFML_CONFIG_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MAJOR ([0-9]+).*" "\\1" SFML_VERSION_MAJOR "${SFML_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MINOR ([0-9]+).*" "\\1" SFML_VERSION_MINOR "${SFML_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SFML_VERSION_PATCH ([0-9]+).*" "\\1" SFML_VERSION_PATCH "${SFML_CONFIG_HPP_CONTENTS}")
    if (NOT "${SFML_VERSION_PATCH}" MATCHES "^[0-9]+$")
        set(SFML_VERSION_PATCH 0)
    endif()
    math(EXPR SFML_REQUESTED_VERSION "${SFML_FIND_VERSION_MAJOR} * 10000 + ${SFML_FIND_VERSION_MINOR} * 100 + ${SFML_FIND_VERSION_PATCH}")

    # If we could extract them, compare with the requested version number
    if (SFML_VERSION_MAJOR)
        # transform version numbers to an integer
        math(EXPR SFML_VERSION "${SFML_VERSION_MAJOR} * 10000 + ${SFML_VERSION_MINOR} * 100 + ${SFML_VERSION_PATCH}")

        # Compare them
        if(SFML_VERSION LESS SFML_REQUESTED_VERSION)
            set(SFML_VERSION_OK FALSE)
        endif()
    else()
        # SFML version is < 2.0
        if (SFML_REQUESTED_VERSION GREATER 10900)
            set(SFML_VERSION_OK FALSE)
            set(SFML_VERSION_MAJOR 1)
            set(SFML_VERSION_MINOR x)
            set(SFML_VERSION_PATCH x)
        endif()
    endif()
endif()

# Find the requested modules
set(SFML_FOUND TRUE) # Will be set to false if one of the required modules is not found
foreach(FIND_SFML_COMPONENT ${SFML_FIND_COMPONENTS})
    string(TOLOWER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_LOWER)
    string(TOUPPER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_UPPER)
    set(FIND_SFML_COMPONENT_NAME sfml-${FIND_SFML_COMPONENT_LOWER})

    # No suffix for sfml-main, it is always a static library
    if(FIND_SFML_COMPONENT_LOWER STREQUAL "main")
        # Release library
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE
                     NAMES ${FIND_SFML_COMPONENT_NAME}
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})

        # Debug library
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG
                     NAMES ${FIND_SFML_COMPONENT_NAME}-d
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
    else()
        # Release library
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
                     NAMES ${FIND_SFML_COMPONENT_NAME}
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})

        # Debug library
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG
                     NAMES ${FIND_SFML_COMPONENT_NAME}-d
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})

        if(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE)
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE})
        endif()
        if(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG})
        endif()
    endif()

    if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG OR SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE)
        # Library found
        set(SFML_${FIND_SFML_COMPONENT_UPPER}_FOUND TRUE)

        # If both are found, set SFML_XXX_LIBRARY to contain only release
        if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG AND SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE)
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE})
        endif()

        # If only one debug/release variant is found, set the other to be equal to the found one
        if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG AND NOT SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE)
            # Debug and not release
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG})
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY         ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG})
        endif()
        if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE AND NOT SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG)
            # Release and not debug
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE})
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY       ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE})
        endif()
    else()
        # Library not found
        set(SFML_FOUND FALSE)
        set(SFML_${FIND_SFML_COMPONENT_UPPER}_FOUND FALSE)
        set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY "")
        set(FIND_SFML_MISSING "${FIND_SFML_MISSING} SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY")
    endif()

    # mark as advanced
    mark_as_advanced(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)

    # Add to the global list of libraries
    set(SFML_LIBRARIES ${SFML_LIBRARIES} "${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY}")
endforeach()

# Handle errors
if(NOT SFML_VERSION_OK)
    # SFML version not ok
    set(FIND_SFML_ERROR "SFML found but version too low (requested: ${SFML_FIND_VERSION}, found: ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH})")
    set(SFML_FOUND FALSE)
elseif(NOT SFML_FOUND)
    # Include directory or library not found
    set(FIND_SFML_ERROR "Could NOT find SFML (missing: ${FIND_SFML_MISSING})")
endif()
if (NOT SFML_FOUND)
    if(SFML_FIND_REQUIRED)
        # Fatal error
        message(FATAL_ERROR ${FIND_SFML_ERROR})
    elseif(NOT SFML_FIND_QUIETLY)
        # Error but continue
        message("${FIND_SFML_ERROR}")
    endif()
endif()

# Handle success
if(SFML_FOUND AND NOT SFML_FIND_QUIETLY)
    message(STATUS "Found SFML ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH} in ${SFML_INCLUDE_DIR}")
endif()

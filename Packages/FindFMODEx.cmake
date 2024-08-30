# Define the list of search paths for headers
find_path(FMODEX_INCLUDE_DIR
    NAMES "fmod.h"
    PATHS
        "${FMODEX_ROOT}"
        "/usr/include"
        "/usr/local/include"
        "/sw/include"
        "/opt/include"
        "/opt/local/include"
        "/opt/csw/include"
        "/opt/fmodex/include"
        "/opt/fmodex/api/inc"
        "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Win32/api/inc"
    PATH_SUFFIXES "" "inc" "include" "fmodex" "fmod" "fmod3"
    DOC "The path to FMOD Ex's include directory."
)

# Define the list of search paths for libraries
find_library(FMODEX_32_LIBRARY
    NAMES "fmodex_vc" "fmodex_bc" "fmodex" "fmodexL" "libfmodex" "libfmodexL" "fmodex_vc" "fmodexL_vc" "libfmodex" "libfmodexL"
    PATHS
        "${FMODEX_ROOT}"
        "/usr"
        "/usr/X11R6"
        "/usr/local"
        "/usr/local/X11R6"
        "/sw"
        "/opt"
        "/opt/local"
        "/opt/csw"
        "/opt/fmodex"
        "/opt/fmodex/api"
        "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Win32/api/lib"
    PATH_SUFFIXES "" "lib" "lib32"
)

find_library(FMODEX_64_LIBRARY
    NAMES "fmodex64" "libfmodex64" "fmodexL64" "libfmodexL64" "fmodex64_vc" "fmodexL64_vc"
    PATHS
        "${FMODEX_ROOT}"
        "/usr"
        "/usr/X11R6"
        "/usr/local"
        "/usr/local/X11R6"
        "/usr/freeware"
        "/sw"
        "/opt"
        "/opt/local"
        "/opt/csw"
        "/opt/fmodex"
        "/opt/fmodex/api"
    PATH_SUFFIXES "" "lib" "lib64"
)

if(FMODEX_32_LIBRARY)
    set(FMODEX_LIBRARY ${FMODEX_32_LIBRARY} CACHE FILEPATH "The filepath to FMOD Ex's library binary.")
elseif(FMODEX_64_LIBRARY)
    set(FMODEX_LIBRARY ${FMODEX_64_LIBRARY} CACHE FILEPATH "The filepath to FMOD Ex's library binary.")
endif()

get_filename_component(FMODEX_LIBRARY_DIR "${FMODEX_LIBRARY}" PATH)
set(FMODEX_LIBRARY_DIR "${FMODEX_LIBRARY_DIR}" CACHE PATH "The path to FMOD Ex's library directory.")

mark_as_advanced(FMODEX_INCLUDE_DIR)
mark_as_advanced(FMODEX_LIBRARY_DIR)
mark_as_advanced(FMODEX_32_LIBRARY)
mark_as_advanced(FMODEX_64_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMODEx DEFAULT_MSG FMODEX_LIBRARY FMODEX_INCLUDE_DIR FMODEX_LIBRARY_DIR)

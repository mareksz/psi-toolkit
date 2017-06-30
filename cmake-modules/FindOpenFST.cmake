# - Try to find OpenFST
# Once done this will define
#
#  OPENFST_FOUND - system has OpenFST
#  OPENFST_INCLUDE_DIR - the include directories for OpenFST

# Paths

find_path(OPENFST_INCLUDE_DIR
    NAMES fst.h
    PATHS /usr/include /usr/local/include
    PATH_SUFFIXES fst)
if (NOT OPENFST_INCLUDE_DIR)
    message(STATUS "Could not find OpenFST include dir")
    set(OPENFST_FOUND FALSE)
endif (NOT OPENFST_INCLUDE_DIR)

# Libraries

find_library(OPENFST_LIBRARY
    NAMES fst libfst
    PATHS /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64)
if (NOT OPENFST_INCLUDE_DIR)
    message(STATUS "Could not find libfst")
    set(OPENFST_FOUND FALSE)
endif (NOT OPENFST_INCLUDE_DIR)


set(OPENFST_INCLUDE_DIRS ${OPENFST_INCLUDE_DIR})
set(OPENFST_LIBRARIES ${OPENFST_LIBRARY})

if (OPENFST_INCLUDE_DIRS AND OPENFST_LIBRARIES)
    set(OPENFST_FOUND TRUE)
else (OPENFST_INCLUDE_DIRS AND OPENFST_LIBRARIES)
    set(OPENFST_FOUND FALSE)
endif (OPENFST_INCLUDE_DIRS AND OPENFST_LIBRARIES)

#.rst:
# FindMuduo
# --------
#
# Find the native muduo includes and library.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   Muduo_INCLUDE_DIRS   - where to find muduo, etc.
#   Muduo_LIBRARIES      - List of libraries when using muduo.
#   Muduo_FOUND          - True if muduo found.
#
# ::
#
#
# Hints
# ^^^^^
#
# A user may set ``MUDUO_ROOT`` to a muduo installation root to tell this
# module where to look.

#=============================================================================
# Copyright 2014-2015 OWenT.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

unset(_MUDUO_SEARCH_ROOT_INC)
unset(_MUDUO_SEARCH_ROOT_LIB)

# Search MUDUO_ROOT first if it is set.
if (Muduo_ROOT)
  set(MUDUO_ROOT ${Muduo_ROOT})
endif()

if(MUDUO_ROOT)
    set(_MUDUO_SEARCH_ROOT_INC PATHS ${MUDUO_ROOT} ${MUDUO_ROOT}/include NO_DEFAULT_PATH)
    set(_MUDUO_SEARCH_ROOT_LIB PATHS ${MUDUO_ROOT} ${MUDUO_ROOT}/lib NO_DEFAULT_PATH)
endif()

# Normal search.
set(Muduo_NAMES muduo libmuduo_base.a libmuduo_base_cpp11.a)

# Try each search configuration.
find_path(Muduo_INCLUDE_DIRS   NAMES muduo            ${_MUDUO_SEARCH_ROOT_INC})
find_library(Muduo_LIBRARIES   NAMES ${Muduo_NAMES}   ${_MUDUO_SEARCH_ROOT_LIB})

mark_as_advanced(Muduo_INCLUDE_DIRS Muduo_LIBRARIES)

# handle the QUIETLY and REQUIRED arguments and set MUDUO_FOUND to TRUE if
# all listed variables are TRUE
include("FindPackageHandleStandardArgs")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Muduo
  REQUIRED_VARS Muduo_INCLUDE_DIRS Muduo_LIBRARIES
  FOUND_VAR Muduo_FOUND
)

if(Muduo_FOUND)
    set(MUDUO_FOUND ${Muduo_FOUND})
endif()

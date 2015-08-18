
#
# CMakeLists.txt
#
# Copyright 2015 Andreas Seuss
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required (VERSION 2.8)
project (BlackElder-Engine)

find_package(OpenGL REQUIRED)

if (OPENGL_FOUND)
    message ("OpenGL found: ${OPENGL_LIBRARIES}")
else (OPENGL_FOUND)
    message ("OpenGL missing")
endif(OPENGL_FOUND)

set (BlackElder-Engine_VERSION_MAJOR 0)
set (BlackElder-Engine_VERSION_MINOR 1)

#
# Set flags to enable warnings on C++ code
#
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CXX_WARNINGS "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (CXX_WARNINGS "/W4 /WX /EHsc")
endif()

#
# Set flags to enable warnings on C code
#
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set (C_WARNINGS "-Wall -Wextra -Werror")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set (C_WARNINGS "/W4 /WX /EHsc")
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARNINGS}")

#
# Check for C compiler flags
#
include (CheckCCompilerFlag)

check_c_compiler_flag ("-std=c99" COMPILER_SUPPORTS_C99)

if (COMPILER_SUPPORTS_C99)
    set (c_flags "${CMAKE_C_FLAGS} -std=c99")
    message (STATUS "Adding -std=c99 to C_FLAGS.")
endif()

#
# Check for C++ compiler flags (especially C++11 support)
#
include (CheckCXXCompilerFlag)

check_cxx_compiler_flag ("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag ("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if (COMPILER_SUPPORTS_CXX11)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11"
    #    CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set (cxx_flags "${CMAKE_CXX_FLAGS} -std=c++11")
    message (STATUS "Adding -std=c++11 to CXX_FLAGS.")
elseif (COMPILER_SUPPORTS_CXX0X)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x"
    #    CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set (cxx_flags "${CMAKE_CXX_FLAGS} -std=c++0x")
    message (STATUS "Adding -std=c++0x to CXX_FLAGS.")
else()
    message (STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#
# Do one time configuration of CMAKE compile flags
#
# TODO: this does not really work out yet
if (NOT CONFIGURED_ONCE)
    set (CMAKE_CXX_FLAGS "${cxx_flags}"
         CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}"
         CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

#find_package(OpenGL REQUIRED)
include_directories (src/)
link_directories (/usr/local/lib)

message ("LD_PATH ${CMAKE_SYSTEM_LIBRARY_PATH}")

add_subdirectory (src/)
add_subdirectory (external/)
add_subdirectory (tests/)
#add_subdirectory (demo/)

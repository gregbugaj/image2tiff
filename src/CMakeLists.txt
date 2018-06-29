cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(image2tiff)
set(TARGET_NAME "image2tiff")

# C++14
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

message("EnvParameter : CMAKE_BUILD_TYPE          = ${CMAKE_BUILD_TYPE}")
message("EnvParameter : CMAKE_SOURCE_DIR          = ${CMAKE_SOURCE_DIR}")
message("EnvParameter : CMAKE_CURRENT_SOURCE_DIR  = ${CMAKE_CURRENT_SOURCE_DIR}")
message("EnvParameter : CMAKE_BINARY_DIR          = ${CMAKE_BINARY_DIR}")
message("EnvParameter : CMAKE_CURRENT_BINARY_DIR  = ${CMAKE_CURRENT_BINARY_DIR}")
message("EnvParameter : CMAKE_INSTALL_PREFIX      = ${CMAKE_INSTALL_PREFIX}")


###########################################################
# Where are the additional libraries installed? Note: provide includes
# path here, subsequent checks will resolve everything else
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_program(GIT "git")
if (GIT)
    execute_process(
            COMMAND git log -1 --format=%H
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else ()
    set(GIT_COMMIT_HASH "unknown")
endif ()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
)

set(SOURCE_FILES main.cpp types.h leptonutil.h leptonutil.cpp cxxopts.hpp)

# Generated
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

# Build as executable
add_executable(${TARGET_NAME} ${SOURCE_FILES})

# Leptonica
include(FindLeptonica)

if (LEPTONICA_FOUND)
	message(STATUS "Leptonica ready")
 	include_directories(${LEPTONICA_INCLUDE_DIRS})
	target_link_libraries(${TARGET_NAME} ${LEPTONICA_LIBRARIES})
else ()
	message(FATAL_ERROR "libleptonica-dev is required install with 'apt-get install libleptonica-dev'")
endif(LEPTONICA_FOUND)


# check for libpng
pkg_check_modules (LIBPNG libpng16 REQUIRED)
if (NOT LIBPNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng16 development libraries installed")
else ()
    include_directories (${LIBPNG_INCLUDE_DIRS})
    link_directories (${LIBPNG_LIBRARY_DIRS})
    link_libraries (${LIBPNG_LIBRARIES})
endif ()


# Filesystem support
target_link_libraries(${TARGET_NAME} stdc++fs)

install(TARGETS image2tiff DESTINATION bin)

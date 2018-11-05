cmake_minimum_required(VERSION 3.7)
project(Captcha)

# Stepping up to CPP 17 breaks OpenCV Currently
set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS -std=c++1z)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp Model helpers)

include(ExternalProject)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    # list of files for which we add a copy rule
    foreach(item SUBDIRLIST(SUBDIRS ${generated_captcha_images}))
        message(STATUS ${item})
        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()

#
#set(CMAKE_C_FLAGS "-Wall -g")
#set(BAMTOOLS_ROOT "${CMAKE_CURRENT_BINARY_DIR}/external")
#set(BAMTOOLS_INCLUDE_DIRS "${BAMTOOLS_ROOT}/src/cppitertools")
##set(BAMTOOLS_LIBRARIES ${BAMTOOLS_ROOT}/lib/cppitertools/libbamtools.a)
#set(BAMTOOLS_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/internal/cppitertools")
##set(bamtools_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${bamtools_INSTALL_DIR})
#
#ExternalProject_Add(cppitertools
#        PREFIX ${BAMTOOLS_ROOT}
#        GIT_REPOSITORY https://github.com/ryanhaining/cppitertools.git
#        CONFIGURE_COMMAND ""
#        BUILD_COMMAND ""
#        INSTALL_COMMAND ""
##        BINARY_DIR ${BAMTOOLS_ROOT}
##        INSTALL_DIR ${BAMTOOLS_ROOT}
##        CMAKE_ARGS ${bamtools_CMAKE_ARGS}
#        )
#
#add_library(libcppitertools INTERFACE)
#add_dependencies(libcppitertools cppitertools)
#include_directories(${BAMTOOLS_INCLUDE_DIRS})

set(EXTERNAL_INSTALL_ROOT "${CMAKE_CURRENT_BINARY_DIR}/external")
set(MLPACK_INCLUDE_DIRS "${EXTERNAL_INSTALL_ROOT}/src/mlpack")
set(MLPACK_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/internal/mlpack")
set(MLPACK_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${MLPACK_INSTALL_DIR}
        -DARMA_EXTRA_DEBUG=OFF
        -DBUILD_CLI_EXECUTABLES=OFF
        -DDEBUG=OFF
        -DPROFILE=OFF
        -DBUILD_PYTHON_BINDINGS=OFF
        -BUILD_TESTS=ON
        -USE_OPENMP=ON)

message(${MLPACK_INCLUDE_DIRS})

find_package(armadillo 6.500.0 REQUIRED)

ExternalProject_Add(mlpack
        PREFIX ${EXTERNAL_INSTALL_ROOT}
        GIT_REPOSITORY https://github.com/mlpack/mlpack.git
#        CONFIGURE_COMMAND ""
#        BUILD_COMMAND bazel build
#        INSTALL_COMMAND ""
#        BINARY_DIR ${BAMTOOLS_ROOT}
#        BUILD_DIR ${TENSORFLOW_INCLUDE_DIRS}
        INSTALL_DIR ${MLPACK_INCLUDE_DIRS}
        CMAKE_ARGS ${MLPACK_CMAKE_ARGS}
        )

add_library(mlpacklib)
add_dependencies(mlpacklib mlpack)
include_directories(${MLPACK_INSTALL_DIR})

find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_package(Boost 1.62.0 COMPONENTS filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS} mlpacklib)
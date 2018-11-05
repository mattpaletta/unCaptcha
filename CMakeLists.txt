cmake_minimum_required(VERSION 3.7)
project(Captcha)

# Stepping up to CPP 17 breaks OpenCV Currently
set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS -std=c++1z)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp Model helpers)

include(ExternalProject)
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

set(EXTERNAL_INSTALL_ROOT "${CMAKE_CURRENT_BINARY_DIR}/externa/")
set(TENSORFLOW_INCLUDE_DIRS "${BAMTOOLS_ROOT}/src/tensorflow")

ExternalProject_Add(tensorflowlib
        PREFIX ${EXTERNAL_INSTALL_ROOT}
        GIT_REPOSITORY https://github.com/tensorflow/tensorflow.git
        CONFIGURE_COMMAND ""
        BUILD_COMMAND basel build
        INSTALL_COMMAND ""
#        BINARY_DIR ${BAMTOOLS_ROOT}
#        INSTALL_DIR ${BAMTOOLS_ROOT}
#        CMAKE_ARGS ${bamtools_CMAKE_ARGS}
        )

add_library(libcppitertools INTERFACE)
add_dependencies(libcppitertools cppitertools)
include_directories(${BAMTOOLS_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_package(Boost 1.62.0 COMPONENTS filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Captcha ${SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS})
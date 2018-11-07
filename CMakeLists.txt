cmake_minimum_required(VERSION 3.7)
project(Captcha CXX)

# Stepping up to CPP 17 breaks OpenCV Currently
set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS -std=c++1z)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(MAKEFLAGS "j5")
#execute_process("export MAKEFLAGS=${MAKEFLAGS}")

set(SOURCE_FILES main.cpp Model helpers)

include(ExternalProject)

#MACRO(SUBDIRLIST result generated_captcha_images)
#    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
#    SET(dirlist "")
#    FOREACH(child ${children})
#        IF(IS_DIRECTORY ${curdir}/${child})
#            LIST(APPEND dirlist ${child})
#        ENDIF()
#    ENDFOREACH()
#    SET(result ${dirlist})
#ENDMACRO()


#if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    # list of files for which we add a copy rule
#endif()
FILE(COPY generated_captcha_images/ DESTINATION "${CMAKE_BINARY_DIR}/generated_captcha_images")

set(EXTERNAL_INSTALL_ROOT "${CMAKE_CURRENT_BINARY_DIR}/external")

#find_package(OpenMP REQUIRED)

set(MOJO_INCLUDE_DIRS ${EXTERNAL_INSTALL_ROOT}/src/mojo/mojo)

ExternalProject_Add(mojo
        PREFIX ${EXTERNAL_INSTALL_ROOT}
        GIT_REPOSITORY https://github.com/gnawice/mojo-cnn.git
        CONFIGURE_COMMAND ""
#        SOURCE_DIR ${MOJO_INCLUDE_DIRS}/mojo
#        BINARY_DIR  ${CMAKE_BINARY_DIR}/mojo-build
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
#        BINARY_DIR ${BAMTOOLS_ROOT}
#        BUILD_DIR ${MOJO_INCLUDE_DIRS}/examples
#        INSTALL_DIR ${MOJO_INCLUDE_DIRS}/examples
        CMAKE_ARGS ${MOJO_CMAKE_ARGS}
        )

#add_library(mojolib)
#add_dependencies(mojolib mojo)
include_directories(${MOJO_INCLUDE_DIRS})


set(MLPACK_INCLUDE_DIRS ${EXTERNAL_INSTALL_ROOT}/src/mlpack)
set(MLPACK_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/internal/mlpack)
set(MLPACK_CMAKE_ARGS ${CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX=${MLPACK_INSTALL_DIR}
        -DARMA_EXTRA_DEBUG=OFF
        -DBUILD_CLI_EXECUTABLES=OFF
        -DDEBUG=OFF
        -DPROFILE=OFF
        -DBUILD_PYTHON_BINDINGS=OFF
        -BUILD_TESTS=OFF
        -USE_OPENMP=OFF)

message(${MLPACK_INCLUDE_DIRS})

find_package(armadillo 6.500.0 REQUIRED)

#find_package(mlpack)
if(mlpack_FOUND)
    else()
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
endif()

add_library(mlpacklib STATIC)
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


target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS})
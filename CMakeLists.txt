cmake_minimum_required(VERSION 3.7)
project(Captcha CXX)

# Stepping up to CPP 17 breaks OpenCV Currently
set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS -std=c++1z)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(MAKEFLAGS "j5")
#execute_process("export MAKEFLAGS=${MAKEFLAGS}")

set(SOURCE_FILES main.cpp Model helpers)

find_package(OpenMP REQUIRED)
FILE(COPY generated_captcha_images/ DESTINATION "${CMAKE_BINARY_DIR}/generated_captcha_images")
FILE(COPY simple_cnn/CNN/ DESTINATION "${CMAKE_BINARY_DIR}/external/src/simple_cnn/CNN/")

include_directories("${CMAKE_BINARY_DIR}/external/src/simple_cnn")

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
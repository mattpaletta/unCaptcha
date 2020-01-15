cmake_minimum_required(VERSION 3.7)
project(Captcha CXX)

# Stepping up to CPP 17 breaks OpenCV Currently
set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS -std=c++1z)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(MAKEFLAGS "j5")
#execute_process("export MAKEFLAGS=${MAKEFLAGS}")

set(SOURCE_FILES main.cpp Model helpers)

FILE(COPY generated_captcha_images/ DESTINATION "${CMAKE_BINARY_DIR}/generated_captcha_images")

find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_package(Boost COMPONENTS filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(DeepCpp)

add_executable(${PROJECT_NAME} lib/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC lib/ include/)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS} deepcpp)

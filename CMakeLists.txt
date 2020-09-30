cmake_minimum_required(VERSION 3.7)
project(Captcha CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## These two things are needed for YCM
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()
  
include(FetchContent)

function(fetch_extern name repo tag)
	FetchContent_Declare(
		${name}
		GIT_REPOSITORY ${repo}
		GIT_TAG ${tag}
	)
	FetchContent_GetProperties(${name})
	if (NOT ${name}_POPULATED)
		FetchContent_Populate(${name})
	endif()

	set(source_dir "${${name}_SOURCE_DIR}")
	set(binary_dir "${${name}_BINARY_DIR}")
	if (EXISTS ${source_dir}/CMakeLists.txt)
		add_subdirectory(${source_dir} ${binary_dir} EXCLUDE_FROM_ALL)
	endif()

	# Store source & binary dir as global variables
	set_property(GLOBAL PROPERTY ${name}_SOURCE_DIR ${source_dir})
	set_property(GLOBAL PROPERTY ${name}_BINARY_DIR ${binary_dir})
endfunction(fetch_extern)


#find_package(OpenCV REQUIRED)

add_subdirectory(DeepCpp)

file(GLOB SRC_FILES lib/*.cpp)
file(GLOB INC_FILES include/*.hpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

#target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} deepcpp)
target_link_libraries(${PROJECT_NAME} deepcpp)

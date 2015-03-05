cmake_minimum_required(VERSION 2.8)

project(platform-independent-layer)

# platform config
if(WIN32)

set(PLATFORM_WIN32 ON)

elseif(UNIX)

set(PLATFORM_LINUX ON)

# Use C++11 in linux
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

endif()

configure_file(
	"${PROJECT_SOURCE_DIR}/platform/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
	)

# option
option(BUILD_EXAMPLES "build examples" ON)

set(PLATFORM_INDEP_LAYER_INC ${PROJECT_SOURCE_DIR}/platform)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_BUILD_TYPE Debug)

link_directories(${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

include_directories(${PLATFORM_INDEP_LAYER_INC})
include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(platform)
add_subdirectory(examples)

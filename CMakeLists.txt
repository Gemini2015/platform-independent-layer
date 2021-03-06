cmake_minimum_required(VERSION 2.8)

project(platform-independent-layer)

set(SYS_LIB_LIST "")

# platform config
if(WIN32)

set(PLATFORM_WIN32 ON)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/deploy/win32" CACHE PATH "Project install directory" FORCE)

elseif(UNIX)

set(PLATFORM_LINUX ON)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/deploy/linux" CACHE PATH "Project install directory" FORCE)

# Use C++11 in linux
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

link_directories(/usr/include/X11)

SET(SYS_LIB_LIST ${SYS_LIB_LIST} "X11")

endif()

# option
option(BUILD_EXAMPLES "build examples" ON)
option(PIL_STATIC "build static lib" FALSE)

set(PIL_STATIC_LIB 0)
if(PIL_STATIC)
	set(PIL_LIB_TYPE STATIC)
	set(PIL_STATIC_LIB 1)
else()
	set(PIL_LIB_TYPE SHARED)
endif()

configure_file(
	"${PROJECT_SOURCE_DIR}/platform/PILConfig.h.in"
	"${PROJECT_BINARY_DIR}/include/PILConfig.h"
	)

set(PLATFORM_INDEP_LAYER_INC ${PROJECT_SOURCE_DIR}/platform)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

link_directories(${PROJECT_BINARY_DIR}/lib)

include_directories(${PLATFORM_INDEP_LAYER_INC})
include_directories(${PROJECT_BINARY_DIR}/include)

add_subdirectory(platform)
add_subdirectory(examples)

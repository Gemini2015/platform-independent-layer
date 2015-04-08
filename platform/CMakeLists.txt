project(platform)

set(PLATFORM_INC
    "PILPrerequisites.h"
	"PILInclude.h"
	"PILRoot.h"
	"PILTime.h"
	"PILFileSystem.h"
	"PILWindow.h"
	"PILWindowManager.h"
	"PILGLUtil.h"
	)

if(WIN32)

set(PLATFORM_SRC
	"win32/PILTime.cpp"
	"win32/PILFileSystem.cpp"
	"win32/PILWindowManager.cpp"
	"win32/PILWindow.cpp"
	"win32/PILGLUtil.cpp"
	)


set(SYS_LIB_LIST ${SYS_LIB_LIST}
	"glew32.lib"
	"glu32.lib"
	"opengl32.lib")

elseif(UNIX)

set(PLATFORM_SRC
	"linux/PILTime.cpp"
	"linux/PILFileSystem.cpp"
	"linux/PILWindowManager.cpp"
	"linux/PILWindow.cpp"
	)

endif()

add_library(${PROJECT_NAME} STATIC ${PLATFORM_INC} ${PLATFORM_SRC})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})

foreach(lib_file platform)
	install(TARGETS ${lib_file}
		RUNTIME DESTINATION lib
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
endforeach()

install(FILES
	${CMAKE_BINARY_DIR}/config.h
	PILPrerequisites.h
	PILInclude.h
	PILRoot.h
	PILFileSystem.h
	PILTime.h
	PILWindow.h
	PILWindowManager.h
	DESTINATION include)
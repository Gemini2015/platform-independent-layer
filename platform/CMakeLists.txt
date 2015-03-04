project(platform)

set(PLATFORM_INC
    "PILPrerequisites.h"
	"PILInclude.h"
	"PILTime.h"
	"PILFileSystem.h"
	"PILWindowSystem.h"
	)

if(WIN32)

set(PLATFORM_SRC
	"win32/PILTime.cpp"
	"win32/PILFileSystem.cpp"
	"win32/PILWindowSystem.cpp"
	)

elseif(UNIX)

set(PLATFORM_SRC
	"linux/PILTime.cpp"
	"linux/PILFileSystem.cpp"
	)

endif()

add_library(${PROJECT_NAME} STATIC ${PLATFORM_INC} ${PLATFORM_SRC})

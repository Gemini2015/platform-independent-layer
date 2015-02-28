project(platform)

set(PLATFORM_INC
	"PILPrerequisites.h"
	"PILTime.h"
	)

if(WIN32)

set(PLATFORM_SRC
	"win32/PILTime.cpp"
	)

elseif(UNIX)

set(PLATFORM_SRC
	""
	)

endif()

add_library(${PROJECT_NAME} STATIC ${PLATFORM_INC} ${PLATFORM_SRC})
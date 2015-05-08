
set(EXAMPLE_NAME "example6")

set(SRC_LIST
	"main.cpp"
	"CustomWindow.cpp"
	"GLDemo.cpp"
	)

set(INC_LIST
	"CustomWindow.h"
	"GLDemo.h"
	)

add_executable(${EXAMPLE_NAME} ${SRC_LIST} ${INC_LIST})
target_link_libraries(${EXAMPLE_NAME} platform)

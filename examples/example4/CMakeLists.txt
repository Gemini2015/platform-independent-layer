# project(example1)

set(EXAMPLE_NAME "example4")

set(SRC_LIST
	"main.cpp"
	"CustomWindow.cpp"
	)

set(INC_LIST
	"CustomWindow.h"
	)

add_executable(${EXAMPLE_NAME} ${SRC_LIST} ${INC_LIST})
target_link_libraries(${EXAMPLE_NAME} platform)

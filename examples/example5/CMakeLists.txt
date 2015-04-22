
set(EXAMPLE_NAME "example5")

set(SRC_LIST
	"main.cpp"
	)

set(INC_LIST
	""
	)

add_executable(${EXAMPLE_NAME} ${SRC_LIST} ${INC_LIST})
target_link_libraries(${EXAMPLE_NAME} platform)

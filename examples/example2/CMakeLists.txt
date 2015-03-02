# project(example2)

set(EXAMPLE_NAME "example2")

set(SRC_LIST
	"main.cpp"
	)

add_executable(${EXAMPLE_NAME} ${SRC_LIST})
target_link_libraries(${EXAMPLE_NAME} platform)
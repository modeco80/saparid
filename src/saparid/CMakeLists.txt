add_executable(saparid
		main.cpp
		)

target_include_directories(saparid PRIVATE ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include)
add_dependencies(saparid _git_version)
saparid_target(saparid)

set_target_properties(saparid PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
		)

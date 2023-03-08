add_executable(saparid
		main.cpp
		)

saparid_target(saparid)
set_target_properties(saparid PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
		)

target_include_directories(saparid PRIVATE ${PROJECT_BINARY_DIR})

target_link_libraries(saparid PRIVATE
	Saparid::Common
)

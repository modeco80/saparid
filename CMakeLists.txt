cmake_minimum_required(VERSION 3.25)

if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source-tree builds are strictly prohibited. Please don't do them.")
endif()

project(saparid
	LANGUAGES CXX
	DESCRIPTION "WIP server for Sony SAPARi"
	VERSION 0.1
	)

# Set the default C++ standard for (bad) projects
# which do not test for feature support.
set(CMAKE_CXX_STANDARD 23)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(Policies)
include(Project)

find_package(Python3 REQUIRED
		COMPONENTS Interpreter)

find_package(Boost REQUIRED COMPONENTS system)

# Third-party dependencies (besides Boost, which is expected to be installed)
add_subdirectory(third_party/kumi)
add_subdirectory(third_party/fmt)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# Target the Windows 10 API set (while it would be nice to support older Windows versions
	# it's more legacy baggage and I'd rather not encumber that right away)
	#
	# TODO: Move this to the project setup calls....
	add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# Add a target for Git version generation.
set(VERSION_FILENAME GitVersion.hpp)
add_custom_target(_git_version
	COMMAND $<TARGET_FILE:Python3::Interpreter> ${PROJECT_SOURCE_DIR}/tools/build/gitversion.py ${VERSION_FILENAME}
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	SOURCES ${PROJECT_BINARY_DIR}/${VERSION_FILENAME})
set_source_files_properties(${PROJECT_BINARY_DIR}/${VERSION_FILENAME} PROPERTIES GENERATED TRUE)


add_subdirectory(src/common)

add_subdirectory(src/saparid)

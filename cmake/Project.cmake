macro(_saparid_target_gcclike target)
	set(_CORE_COMPILE_ARGS -Wall -Wextra)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(_CORE_COMPILE_ARGS ${_CORE_COMPILE_ARGS} -Werror)
	endif()
endmacro()

macro(_saparid_target_msvc target)
	set(_CORE_COMPILE_ARGS /W4)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(_CORE_COMPILE_ARGS ${_CORE_COMPILE_ARGS} /WX)
	endif()
endmacro()

function(saparid_target target)
	target_compile_definitions(${target} PRIVATE "$<$<CONFIG:DEBUG>:SAPARID_DEBUG>")
	target_compile_features(${target} PUBLIC cxx_std_20)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		_saparid_target_gcclike(${target})
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		_saparid_target_msvc(${target})
	else()
		message(FATAL_ERROR "I do not know how to setup ${CMAKE_CXX_COMPILER_ID}")
	endif()

	# default compile options to the core compile flags set
	# by the macro implementation
	target_compile_options(${target} PRIVATE ${_CORE_COMPILE_ARGS})
endfunction()

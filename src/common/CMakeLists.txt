add_library(saparid_common
		AsioImpl.cpp
		)

saparid_target(saparid_common)

target_link_libraries(saparid_common PUBLIC
		Boost::system
		kumi::kumi
		fmt::fmt
		)
		
target_include_directories(saparid_common PRIVATE ${PROJECT_BINARY_DIR})
target_include_directories(saparid_common PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_definitions(saparid_common PUBLIC
		# ASIO options
		# We compile Asio in another TU (provided in this library)
		-DBOOST_ASIO_SEPARATE_COMPILATION=1
		
		# Disable deprecated functionality
		-DBOOST_ASIO_NO_DEPRECATED=1

		# Disable some things which add additional dependencies or are 
		# simply baggage we aren't ever going to use
		-DBOOST_ASIO_DISABLE_BOOST_ARRAY=1
		-DBOOST_ASIO_DISABLE_BOOST_BIND=1
		-DBOOST_ASIO_DISABLE_BOOST_DATE_TIME=1
		-DBOOST_ASIO_DISABLE_BOOST_REGEX=1
		)

add_library(Saparid::Common ALIAS saparid_common)

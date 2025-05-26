if(BUILD_TESTING)
	# Set up subdirectories for tests
	add_subdirectory(http)
	add_subdirectory(network)
	add_subdirectory(mime)
	add_subdirectory(response)
	add_subdirectory(parser)
	add_subdirectory(utils)

	# Create test executable with all test files
	add_executable(webreq
			main.cpp
			
			# Core tests
			${UTILS_TEST_SOURCES}
			
			# HTTP related tests
			${HTTP_TEST_SOURCES}
			
			# Network related tests
			${NETWORK_TEST_SOURCES}
			
			# MIME tests
			${MIME_TEST_SOURCES}
			
			# Response related tests
			${RESPONSE_TEST_SOURCES}
			
			# Parser tests
			${PARSER_TEST_SOURCES}
			
			# File related tests
			${FILE_TEST_SOURCES}
	)

	# Include directories for tests
	target_include_directories(webreq
			PRIVATE
			${CMAKE_SOURCE_DIR}/src
			${CMAKE_SOURCE_DIR}/tests
			${CMAKE_SOURCE_DIR}/tests/mocks
	)

	# Link libraries
	target_link_libraries(webreq
			PRIVATE
			GTest::gtest_main
			gtest
			gtest_main
			webreq_lib
			gmock
	)

	# Add a custom command to copy resource files for tests
	add_custom_command(TARGET webreq POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
			${CMAKE_CURRENT_BINARY_DIR}/resources/configurations
			${CMAKE_CURRENT_BINARY_DIR}/resources/src/parser
		COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/configurations/default.conf
			${CMAKE_CURRENT_BINARY_DIR}/resources/configurations/
		COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/src/parser/mime/mimes.json
			${CMAKE_CURRENT_BINARY_DIR}/resources/src/parser/
		COMMENT "Copying resource files for tests"
	)

	include(GoogleTest)
	gtest_discover_tests(webreq)
endif()

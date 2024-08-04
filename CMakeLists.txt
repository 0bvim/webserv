cmake_minimum_required(VERSION 3.10)
project(MyWebServer VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include) 

# Find the Google Test package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add the main application executable
add_executable(my_web_server src/main.cpp src/Client.cpp src/Config.cpp src/ConfigUtils.cpp src/InitServer.cpp src/Request.cpp src/Response.cpp src/Server.cpp src/ServerUtils.cpp src/utils.cpp)

# Specify any dependencies for the main application here
# target_link_libraries(my_web_server dependency_name)

# Define the test executable
add_executable(run_tests tests/main.cpp tests/testConfig.cpp tests/testItoa.cpp tests/testRequest.cpp tests/testServer.cpp)

# Link the test executable against Google Test
target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)

target_include_directories(run_tests PRIVATE include ${GTEST_INCLUDE_DIRS}) # This ensures 'include' and GTest directories are included specifically for run_tests

# Optionally, enable CMake's built-in test discovery
enable_testing()
add_test(NAME MyWebServerTests COMMAND run_tests)

# Common CMake settings for all test directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/tests/mocks
)

macro(register_tests CATEGORY)

    set(${CATEGORY}_TEST_SOURCES "")
    
    file(GLOB CATEGORY_SOURCES *.cpp)
    
    list(APPEND ${CATEGORY}_TEST_SOURCES ${CATEGORY_SOURCES})
    
    set(${CATEGORY}_TEST_SOURCES ${${CATEGORY}_TEST_SOURCES} PARENT_SCOPE)
endmacro() 
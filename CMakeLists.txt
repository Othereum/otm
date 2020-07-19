cmake_minimum_required(VERSION 3.8)
project("otm")

add_library(otm INTERFACE)
target_include_directories(otm INTERFACE "include")

set(OTM_DEFAULT_FLOAT "float" CACHE STRING "Default floating point type")
target_compile_definitions(otm INTERFACE "OTM_DEFAULT_FLOAT=${OTM_DEFAULT_FLOAT}")

set(OTM_BUILD_TESTS FALSE CACHE BOOL "Whether to build a test")
if(OTM_BUILD_TESTS)
	file(GLOB_RECURSE TEST_SRC_FILES "tests/*.cpp")
	add_executable(otm_test ${TEST_SRC_FILES})
	set_target_properties(otm_test PROPERTIES CXX_STANDARD 17)

	find_package(GTest 1.10.0 REQUIRED)
	target_link_libraries(otm_test PRIVATE otm GTest::GTest)

	enable_testing()
	add_test(NAME "otm test" COMMAND otm_test)
endif()

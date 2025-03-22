include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)
print_header("Configuring Unit Tests for Project ${PROJECT_NAME}")

include(${CMAKE_MODULE_PATH}/pkgs/gtest_install.cmake)

file(GLOB_RECURSE  TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
set(TEST_SRC_FILES ${TEST_SOURCES})
file(GLOB_RECURSE  TEST_UTIL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp)
set(UTIL_SRC_FILES ${TEST_UTIL_SOURCES})

add_executable(unit_tests
	${TEST_SRC_FILES}
	${UTIL_SRC_FILES}
)
target_include_directories(unit_tests PRIVATE include)
target_link_libraries(unit_tests PRIVATE
	template_lib
	gtest
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)


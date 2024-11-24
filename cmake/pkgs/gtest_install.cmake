set(GTEST_VERSION "1.13.0" CACHE STRING "GoogleTest Version to Install (Branch/Tag/Release)")

include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)

print_header("Configuring GoogleTest Library version: ${EIGEN_VERSION}")

find_package(GTest ${GTEST_VERSION} NO_MODULE)

if (NOT GTest_FOUND)
	message(STATUS "--> Unable to find GoogleTest locally - Fetching source")

	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)

	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL "https:://github.com/google/googletest/archive/refs/tags/v${GTEST_VERSION}.tar.gz"
	)
	FetchContent_MakeAvailable(googletest)
	add_library(GTest::gtest ALIAS gtest)
	add_library(GTest::gtest_main ALIAS gtest_main)
	add_library(GTest::gmock ALIAS gmock)
	add_library(GTest::gmock_main ALIAS gmock_main)
else()
	message(STATUS "--> Using locally installed GoogleTest")
endif()

set(EIGEN_VERSION "3.4.0" CACHE STRING "Eigen Version to Install (Branch/Tag/Release)")

include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)

print_header("Configuring Eigen Library version: ${EIGEN_VERSION}")

find_package(Eigen3 ${EIGEN_VERSION} NO_MODULE)

if (NOT Eigen3_FOUND)
	message(STATUS "--> Unable to find Eigen locally - Fetching source")

	set(BUILD_TESTING OFF CACHE BOOL "Disable building of eigen unit tests" FORCE)
	set(EIGEN_BUILD_BTL OFF CACHE BOOL "Disable building of benchmark suite" FORCE)

	include(FetchContent)
	FetchContent_Declare(
		eigen3
		URL "https://gitlab.com/libeigen/eigen/-/archive/${EIGEN_VERSION}/eigen-${EIGEN_VERSION}.tar.gz"
	)
	FetchContent_MakeAvailable(eigen3)
	add_library(Eigen3::Eigen ALIAS eigen)
else()
	message(STATUS "--> Using locally installed Eigen")
endif()

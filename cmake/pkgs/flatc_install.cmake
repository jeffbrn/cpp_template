set(FLATC_VERSION "23.5.26" CACHE STRING "Flatbuffers Version to Install (Branch/Tag/Release)")

include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)

print_header("Configuring Flatbuffers Library version: ${FLATC_VERSION}")

find_package(flatbuffers ${FLATC_VERSION} EXACT CONFIG)

if (NOT flatbuffers_FOUND)
	message(STATUS "--> Unable to find flatbuffers locally - Fetching source")

	set(FLATBUFFERS_BUILD_TESTS OFF CACHE INTERNAL "")

	include(FetchContent)
	FetchContent_Declare(
		flatbuffers
		URL "https://github.com/google/flatbuffers/archive/refs/tags/v${FLATC_VERSION}.tar.gz"
	)
	FetchContent_MakeAvailable(flatbuffers)
else()
	message(STATUS "--> Using locally installed Flatbuffers")
endif()

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

# print_header("Compiling IDL Definitions")

# file(GLOB_RECURSE IDL_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/idl/*.fbs)
# set(IDL_SRCS ${IDL_SRC_LIST})

# set(IDL_HDR_DIR "${CMAKE_CURRENT_BINARY_DIR}/idl_compile" CACHE STRING "Directory for generated IDL headers")

# flatbuffers_generate_headers(
# 	TARGET idl_compile
# 	SCHEMAS ${IDL_SRCS}
# 	INCLUDE_PREFIX "messages"
# 	)

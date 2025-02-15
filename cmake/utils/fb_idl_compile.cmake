include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)

print_header("Compiling IDL Definitions")

file(GLOB_RECURSE IDL_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/idl/*.fbs)
set(IDL_SRCS ${IDL_SRC_LIST})

set(IDL_HDR_DIR "${CMAKE_CURRENT_BINARY_DIR}/idl_compile" CACHE STRING "Directory for generated IDL headers")

flatbuffers_generate_headers(
	TARGET idl_compile
	SCHEMAS ${IDL_SRCS}
	INCLUDE_PREFIX "messages"
	)
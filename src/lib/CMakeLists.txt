include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)
print_header("Configuring Project Library")

set(LIB_NAME template_lib)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(SRC_FILES ${SOURCES})

include(${CMAKE_MODULE_PATH}/pkgs/eigen3_install.cmake)
include(${CMAKE_MODULE_PATH}/pkgs/flatc_install.cmake)

add_library(${LIB_NAME}_static STATIC
	${SRC_FILES}
)
target_include_directories(${LIB_NAME}_static
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${LIB_NAME}_static PRIVATE Eigen3::Eigen flatbuffers)

add_library(${LIB_NAME} SHARED
	${SRC_FILES}
)
target_include_directories(${LIB_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${LIB_NAME} PRIVATE Eigen3::Eigen flatbuffers)

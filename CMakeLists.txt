cmake_minimum_required(VERSION 3.16)

# Set your project name here
project( cpp_template VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "Project Description here")

# Set the C++ project properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=^(?!build/_deps/).*")

# Project Options
option(BUILD_TESTS "BUILD_TESTS Enables building the unit test code for this project" ON)

include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)

add_subdirectory(src/lib)

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

# Configure main executable
print_header("Configuring Project ${PROJECT_NAME}")

# Create an executable target from the source files
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
# Set any additional compiler flags if needed
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wfatal-errors)
target_link_libraries(${PROJECT_NAME}
	PUBLIC template_lib_static
	)

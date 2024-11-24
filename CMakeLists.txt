cmake_minimum_required(VERSION 3.16)

# Set your project name here
project( cpp_template VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "Project Description here")

# Set the C++ project properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(${CMAKE_MODULE_PATH}/utils/debugging.cmake)

# Configure main executable
print_header("Configuring Project ${PROJECT_NAME}")
# Create an executable target from the source files
add_executable(${PROJECT_NAME} src/main.cpp src/func.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
# Set any additional compiler flags if needed
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

include(${CMAKE_MODULE_PATH}/pkgs/eigen3_install.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)

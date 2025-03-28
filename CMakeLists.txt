cmake_minimum_required(VERSION 3.14)
project(GameEngine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build test applications" OFF)

# Compiler specific options
if(MSVC)
    # MSVC specific options
    add_compile_options(/W4 /MP)
else()
    # GCC/Clang options
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dependencies
find_package(OpenGL REQUIRED)
# Add other dependencies as needed
# find_package(GLFW REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(assimp REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # Add other include directories as needed
)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Engine/*.hpp
)

# Main engine library
add_library(Engine ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Engine
    PUBLIC
    OpenGL::GL
    # Add other libraries as needed
    # GLFW::GLFW
    # GLEW::GLEW
    # assimp::assimp
)

# Install targets
install(TARGETS Engine
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

# Build examples if enabled
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build tests if enabled
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# GLAD
add_subdirectory(external/glad)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)

# STB Image
add_subdirectory(external/stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/stb)

# Update Engine library target
target_link_libraries(Engine
    PUBLIC
    OpenGL::GL
    glfw
    glad
    stb
)
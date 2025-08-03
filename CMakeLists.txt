# CMake minimum version and project definition
cmake_minimum_required(VERSION 3.16)
project(MyGame VERSION 1.0 LANGUAGES CXX)

# Set C++ standard (C++17 is a good choice)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# --- Add Flecs ---
# Add the flecs submodule directory. This creates the 'flecs' target.
add_subdirectory(lib/flecs)

# --- Find SDL2 ---
# Find the core SDL2 library
find_package(SDL2 REQUIRED)
# Find optional SDL2 helper libraries
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories for SDL2
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# --- Project Files ---
# Gather all your source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the executable for your game
add_executable(MyGame ${SOURCES})

# Include your project's header directory
target_include_directories(MyGame PRIVATE "${PROJECT_SOURCE_DIR}/include")

# --- Link Libraries ---
# Link your game against Flecs and SDL2
target_link_libraries(MyGame PRIVATE
    flecs           # Link Flecs
    ${SDL2_LIBRARIES}     # Link core SDL2
    ${SDL2_IMAGE_LIBRARIES} # Link SDL2_image
    ${SDL2_TTF_LIBRARIES}   # Link SDL2_ttf
)

# --- Optional: A message to confirm setup ---
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Flecs sources added from subdirectory.")
message(STATUS "SDL2 Libraries: ${SDL2_LIBRARIES}")

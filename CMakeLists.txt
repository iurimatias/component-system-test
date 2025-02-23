cmake_minimum_required(VERSION 3.16)
project(ComponentSystemDemo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Build the component system first
add_subdirectory(vendor/componentsystem)

# Set up include paths for the component system
include_directories(
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/src
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/include
)

# Add components and main app
add_subdirectory(components/counter)
add_subdirectory(components/colorchanger)
add_subdirectory(mainapp)

# Make sure mainapp depends on ComponentSystem
add_dependencies(mainapp ComponentSystem)
add_dependencies(counter ComponentSystem)
add_dependencies(colorchanger ComponentSystem) 
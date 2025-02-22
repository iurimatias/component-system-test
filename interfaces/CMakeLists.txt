cmake_minimum_required(VERSION 3.16)
project(ComponentInterfaces VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Create interface library
add_library(component-interfaces INTERFACE)
target_include_directories(component-interfaces INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(component-interfaces INTERFACE Qt6::Core Qt6::Widgets) 
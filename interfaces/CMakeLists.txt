cmake_minimum_required(VERSION 3.16)
project(ComponentInterfaces VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the path to Qt installation
# set(Qt6_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6")
# set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.8.2")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Create interface library
add_library(component-interfaces INTERFACE)

target_include_directories(component-interfaces 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
)

target_link_libraries(component-interfaces 
    INTERFACE 
        Qt6::Core 
        Qt6::Widgets
) 
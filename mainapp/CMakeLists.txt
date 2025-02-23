cmake_minimum_required(VERSION 3.16)
project(ComponentSystemDemo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the path to Qt installation
# set(Qt6_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6")
# set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.8.2")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Try to find the component-interfaces package first
find_package(component-interfaces QUIET)

# If not found, build it from source
if(NOT component-interfaces_FOUND)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../interfaces interfaces_build)
endif()

set(MAINAPP_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

add_executable(mainapp ${MAINAPP_SOURCES})

# Add include directories for component system
target_include_directories(mainapp PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/src
)

target_link_libraries(mainapp 
    PRIVATE 
        Qt6::Core 
        Qt6::Widgets
        ComponentSystem
)

# Set the output directory for the executable
set_target_properties(mainapp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
) 
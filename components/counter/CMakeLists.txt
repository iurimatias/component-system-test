cmake_minimum_required(VERSION 3.16)
project(CounterPlugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add the interfaces directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces ${CMAKE_BINARY_DIR}/interfaces)

# Add the root directory to include path for interface headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(SOURCES
    CounterComponent.cpp
    CounterWidget.cpp
)

add_library(counter SHARED ${SOURCES})
target_link_libraries(counter 
    PRIVATE 
        Qt6::Core 
        Qt6::Widgets
        component-interfaces
)

# For macOS, we need to set the bundle properties
if(APPLE)
    set_target_properties(counter PROPERTIES
        BUNDLE FALSE
        FRAMEWORK FALSE
        PREFIX ""
        SUFFIX ".dylib"
    )
else()
    set_target_properties(counter PROPERTIES
        PREFIX "lib"
        SUFFIX ".so"
    )
endif() 
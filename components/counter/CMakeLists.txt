cmake_minimum_required(VERSION 3.16)
project(CounterComponent VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add definition for exporting symbols
add_definitions(-DNEDRYSOFT_COMPONENT_COUNTER_EXPORT)

set(SOURCES
    CounterComponent.cpp
    CounterComponent.h
    CounterWidget.cpp
    CounterWidget.h
    CounterSpec.h
)

add_library(counter SHARED ${SOURCES})

target_include_directories(counter PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/src
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/include
)

target_link_libraries(counter
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        ComponentSystem
)

# Set output directory for the component
set_target_properties(counter PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/components"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/components"
)

# Configure metadata
configure_file(metadata.json.in "${CMAKE_BINARY_DIR}/bin/components/counter/metadata.json" @ONLY) 
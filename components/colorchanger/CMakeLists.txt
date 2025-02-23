cmake_minimum_required(VERSION 3.16)
project(ColorChangerComponent VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add definition for exporting symbols
add_definitions(-DNEDRYSOFT_COMPONENT_COLORCHANGER_EXPORT)

set(SOURCES
    ColorChangerComponent.cpp
    ColorChangerComponent.h
    ColorChangerWidget.cpp
    ColorChangerWidget.h
    ColorChangerSpec.h
)

add_library(colorchanger SHARED ${SOURCES})

target_include_directories(colorchanger PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/src
    ${CMAKE_SOURCE_DIR}/vendor/componentsystem/include
)

target_link_libraries(colorchanger
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        ComponentSystem
)

# For macOS, we need to set the bundle properties
if(APPLE)
    set_target_properties(colorchanger PROPERTIES
        BUNDLE FALSE
        FRAMEWORK FALSE
        PREFIX ""
        SUFFIX ".dylib"
    )
else()
    set_target_properties(colorchanger PROPERTIES
        PREFIX "lib"
        SUFFIX ".so"
    )
endif()

# Set output directory for the component
set_target_properties(colorchanger PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/components"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/components"
)

# Configure metadata
configure_file(metadata.json.in "${CMAKE_BINARY_DIR}/bin/components/colorchanger/metadata.json" @ONLY) 
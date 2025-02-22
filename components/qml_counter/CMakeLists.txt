cmake_minimum_required(VERSION 3.16)
project(QMLCounterPlugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the path to Qt installation
set(Qt6_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6")
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.8.2")

# Force using homebrew Qt
set(Qt6_ROOT "/opt/homebrew/Cellar/qt/6.8.2")
set(Qt6Core_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6Core")
set(Qt6Widgets_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6Widgets")
set(Qt6Quick_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6Quick")
set(Qt6QuickWidgets_DIR "/opt/homebrew/Cellar/qt/6.8.2/lib/cmake/Qt6QuickWidgets")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets)
find_package(component-interfaces QUIET)

if(NOT component-interfaces_FOUND)
    add_subdirectory(../../interfaces interfaces)
endif()

set(SOURCES
    QMLCounterComponent.cpp
    resources.qrc
)

add_library(qml_counter SHARED ${SOURCES})

target_include_directories(qml_counter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6Quick_INCLUDE_DIRS}
    ${Qt6QuickWidgets_INCLUDE_DIRS}
)

target_link_libraries(qml_counter PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickWidgets
    component-interfaces
)

if(APPLE)
    set_target_properties(qml_counter PROPERTIES
        SUFFIX ".dylib"
        PREFIX "lib"
    )
else()
    set_target_properties(qml_counter PROPERTIES
        PREFIX ""
        SUFFIX ".so"
    )
endif()

# Create plugins directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../plugins)

# Set the output directory for the plugin
set_target_properties(qml_counter PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../plugins
) 
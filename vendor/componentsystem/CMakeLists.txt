#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of the Nedrysoft component system (https://github.com/nedrysoft/componentsystem)
#
# A cross-platform plugin system for Qt applications.
#
# Created by Adrian Carpenter on 03/12/2020.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(SET CMP0077 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-DQT_NO_KEYWORDS)

add_definitions(-DNEDRYSOFT_LIBRARY_COMPONENTSYSTEM_EXPORT)

include_directories(${CMAKE_BINARY_DIR})    # <- allows ui_ files to be available to the editor

# create the component system library

project(ComponentSystem)

set(library_SOURCES
    src/Component.cpp
    src/Component.h
    src/ComponentLoader.cpp
    src/ComponentLoader.h
    src/ComponentSystemSpec.h
    src/IComponent.cpp
    src/IComponent.h
    src/IComponentManager.cpp
    src/IComponentManager.h
    src/IInterface.h
)

if(WIN32)
    configure_file("Version.h.in" "Version.h")
    list(APPEND library_SOURCES "version.rc")
    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${PROJECT_NAME}.dll\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${PROJECT_NAME}\"")
endif()

# discover which Qt version is available

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

set(Qt_LIBS Qt6::Core Qt6::Widgets)

include_directories("src" "include")

# Always enable component viewer
set(NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER ON)

set(Viewer_SOURCES
    src/ComponentDetailsDialog.h
    src/ComponentViewerDialog.h
    src/ComponentDetailsDialog.cpp
    src/ComponentDetailsDialog.ui
    src/ComponentViewerDialog.cpp
    src/ComponentViewerDialog.ui
    src/ComponentSystemFontAwesome.cpp
    src/ComponentSystemFontAwesome.h
    src/ComponentSystemFontAwesome.qrc
)

add_library(${PROJECT_NAME} SHARED
    ${library_SOURCES}
    ${Viewer_SOURCES}
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})

# Install locally
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    src/Component.h
    src/ComponentLoader.h
    src/ComponentSystemSpec.h
    src/IComponent.h
    src/IComponentManager.h
    src/IInterface.h
    src/ComponentViewerDialog.h
    DESTINATION include/ComponentSystem
)

# Generate and install CMake config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/NedrysoftComponentSystemConfig.cmake"
    INSTALL_DESTINATION lib/cmake/NedrysoftComponentSystem
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/NedrysoftComponentSystemConfig.cmake"
    DESTINATION lib/cmake/NedrysoftComponentSystem
)

cmake_minimum_required(VERSION 3.16)
project(ComponentSystemDemo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add subdirectories
add_subdirectory(components/counter)
add_subdirectory(components/colorchanger)
add_subdirectory(mainapp) 
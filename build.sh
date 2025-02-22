#!/bin/bash

# Exit on any error
set -e

# Function to print usage
print_usage() {
    echo "Usage: $0 [component]"
    echo "Components:"
    echo "  main         - Build main application only"
    echo "  counter      - Build counter plugin only"
    echo "  colorchanger - Build color changer plugin only"
    echo "  qml_counter  - Build QML counter plugin only"
    echo "  all          - Build everything (default)"
    echo ""
    echo "Examples:"
    echo "  $0           - Same as '$0 all'"
    echo "  $0 main      - Build only the main application"
    echo "  $0 counter   - Build only the counter plugin"
}

# Function to build main application
build_main() {
    echo "Building main application..."
    mkdir -p build/app
    cd build/app
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    cmake ../../mainapp
    cmake --build .
    mkdir -p plugins
    echo "Main application built successfully!"
}

# Function to build counter plugin
build_counter() {
    echo "Building counter plugin..."
    mkdir -p build/counter
    cd build/counter
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    cmake ../../components/counter
    cmake --build .
    cp counter.dylib ../app/plugins/
    echo "Counter plugin built successfully!"
    echo "Plugin location: $(pwd)/../app/plugins/counter.dylib"
}

# Function to build colorchanger plugin
build_colorchanger() {
    echo "Building color changer plugin..."
    mkdir -p build/colorchanger
    cd build/colorchanger
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    cmake ../../components/colorchanger
    cmake --build .
    cp colorchanger.dylib ../app/plugins/
    echo "Color changer plugin built successfully!"
    echo "Plugin location: $(pwd)/../app/plugins/colorchanger.dylib"
}

# Function to build QML counter plugin
build_qml_counter() {
    echo "Building QML counter plugin..."
    mkdir -p build/qml_counter
    cd build/qml_counter
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    cmake ../../components/qml_counter
    cmake --build .
    cp ../plugins/libqml_counter.dylib ../app/plugins/
    echo "QML counter plugin built successfully!"
    echo "Plugin location: $(pwd)/../app/plugins/libqml_counter.dylib"
}

# Function to build everything
build_all() {
    echo "Building all components..."
    build_main
    cd ../..
    build_counter
    cd ../..
    build_colorchanger
    cd ../..
    build_qml_counter
    echo "All components built successfully!"
}

# Create build directory if it doesn't exist
mkdir -p build

# Handle command line argument
case "${1:-all}" in
    "main")
        build_main
        ;;
    "counter")
        build_counter
        ;;
    "colorchanger")
        build_colorchanger
        ;;
    "qml_counter")
        build_qml_counter
        ;;
    "all")
        build_all
        ;;
    "help"|"-h"|"--help")
        print_usage
        ;;
    *)
        echo "Error: Unknown component '${1}'"
        echo ""
        print_usage
        exit 1
        ;;
esac
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
    echo "  all          - Build everything (default)"
    echo ""
    echo "Examples:"
    echo "  $0           - Same as '$0 all'"
    echo "  $0 main      - Build only the main application"
    echo "  $0 counter   - Build only the counter plugin"
}

# Function to build everything (default)
build_all() {
    echo "Building all components..."
    # Remove existing build directory
    rm -rf build
    mkdir -p build
    cd build
    
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    
    # Build everything
    cmake ..
    make
    
    echo "All components built successfully!"
    echo "Binary location: $(pwd)/bin/mainapp"
    echo "Plugins location: $(pwd)/bin/components/"
}

# Function to build main application only
build_main() {
    echo "Building main application..."
    mkdir -p build
    cd build
    
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    
    # Build main app and component system
    cmake .. -DBUILD_COMPONENTS=OFF
    make mainapp
    
    echo "Main application built successfully!"
    echo "Binary location: $(pwd)/bin/mainapp"
}

# Function to build counter plugin only
build_counter() {
    echo "Building counter plugin..."
    mkdir -p build
    cd build
    
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    
    # Build counter component
    cmake .. -DBUILD_MAIN=OFF -DBUILD_COLORCHANGER=OFF
    make counter
    
    echo "Counter plugin built successfully!"
    echo "Plugin location: $(pwd)/bin/components/libcounter.dylib"
}

# Function to build colorchanger plugin only
build_colorchanger() {
    echo "Building color changer plugin..."
    mkdir -p build
    cd build
    
    # Force using homebrew Qt
    export PATH="/opt/homebrew/bin:$PATH"
    export CMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt/6.8.2"
    
    # Build colorchanger component
    cmake .. -DBUILD_MAIN=OFF -DBUILD_COUNTER=OFF
    make colorchanger
    
    echo "Color changer plugin built successfully!"
    echo "Plugin location: $(pwd)/bin/components/colorchanger.dylib"
}

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
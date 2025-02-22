#!/bin/bash

# Exit on any error
set -e

# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

echo "Build completed successfully!"
echo "To run the application, use: ./build/mainapp/mainapp" 
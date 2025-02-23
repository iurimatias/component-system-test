#!/bin/bash

# Exit on any error
set -e

# Check if the application exists
if [ ! -f "build/bin/mainapp" ]; then
    echo "Application not found. Building all components..."
    ./build.sh all
fi

# Run the application from the build directory
cd build/bin
./mainapp
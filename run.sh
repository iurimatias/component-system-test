#!/bin/bash

# Exit on any error
set -e

# Check if the application exists
if [ ! -f "build/mainapp/mainapp" ]; then
    echo "Application not found. Building first..."
    ./build.sh
fi

# Run the application from the build directory
cd build
./mainapp/mainapp
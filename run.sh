#!/bin/bash

# Exit on any error
set -e

# Check if the application exists
if [ ! -f "build/app/mainapp" ]; then
    echo "Application not found. Building main application..."
    ./build.sh main
fi

# Create plugins directory if it doesn't exist
mkdir -p build/plugins
mkdir -p build/app/plugins

# Run the application from the build directory
cd build/app
./mainapp
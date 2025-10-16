#!/bin/bash
# ===========================================
# File: build.sh
# Description: Script to execute the CMake build process.
# This must be run by the Jenkins 'Build' stage.
# ===========================================

set -e # Exit immediately if a command exits with a non-zero status.

BUILD_DIR="build"

echo "Creating build directory: $BUILD_DIR"
mkdir -p $BUILD_DIR

echo "Running CMake to generate build files..."
# Run cmake from inside the build directory, pointing to the project root (..)
cd $BUILD_DIR
cmake ..

echo "Running Make to compile the project and tests..."
# Compile all targets defined in CMakeLists.txt (main_app and unit_tester)
make

echo "Build complete. Executable(s) created in $BUILD_DIR."
# Note: The 'compile_commands.json' needed by SonarQube is now also in the 'build' directory.

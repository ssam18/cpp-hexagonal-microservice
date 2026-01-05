#!/bin/bash

# Build script for local development (without Docker)

set -e

echo "==================================="
echo "Building Product Catalog Service"
echo "==================================="
echo ""

# Check if vcpkg is installed
if [ ! -d "$VCPKG_ROOT" ]; then
    echo "Error: VCPKG_ROOT is not set or vcpkg is not installed."
    echo "Please install vcpkg and set VCPKG_ROOT environment variable."
    echo ""
    echo "Installation instructions:"
    echo "  git clone https://github.com/Microsoft/vcpkg.git"
    echo "  cd vcpkg"
    echo "  ./bootstrap-vcpkg.sh"
    echo "  export VCPKG_ROOT=\$(pwd)"
    exit 1
fi

echo "Installing dependencies with vcpkg..."
$VCPKG_ROOT/vcpkg install

echo ""
echo "Configuring CMake..."
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

echo ""
echo "Building..."
cmake --build build --config Release

echo ""
echo "Build completed successfully!"
echo "Executable: ./build/ProductCatalogService"
echo ""
echo "To run the service, make sure MongoDB is running and set environment variables:"
echo "  export SERVER_ADDRESS=0.0.0.0"
echo "  export SERVER_PORT=8080"
echo "  export MONGO_URI=mongodb://localhost:27017"
echo "  export DATABASE_NAME=product_catalog"
echo "  ./build/ProductCatalogService"

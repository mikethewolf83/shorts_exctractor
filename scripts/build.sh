#!/bin/bash
# Build script for Linux/macOS

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

echo ""
echo "Build complete. Executable in $BUILD_DIR/shorts_extractor"
ls -la shorts_extractor

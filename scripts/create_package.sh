#!/bin/bash

# Exit on error
set -e

# Store the root directory
ROOT_DIR="$(pwd)"
VERSION="0.0.1"

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure and build in Release mode
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Create package directory structure
PACKAGE_DIR="package"
PLUGIN_NAME="Bass Fukz.vst3"
INSTALL_DIR="/Library/Audio/Plug-Ins/VST3"

# Create package directory
mkdir -p "$PACKAGE_DIR"

# Copy the VST3 plugin to the package directory
cp -R "ArchitextureStudiosAnalogCore_artefacts/Release/VST3/$PLUGIN_NAME" "$PACKAGE_DIR/"

# Change back to root directory
cd "$ROOT_DIR"

# Create the package
pkgbuild --root "build/$PACKAGE_DIR" \
         --install-location "$INSTALL_DIR" \
         --identifier "com.architexturestudios.bassfukz" \
         --version "$VERSION" \
         "BassFukz-$VERSION.pkg"

echo "Package created: BassFukz-$VERSION.pkg" 
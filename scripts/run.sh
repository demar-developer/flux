#!/bin/bash
# FLUX — Run the application

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

BUILD_DIR="${PROJECT_ROOT}/build"
BUILD_TYPE="${1:-Release}"

echo "FLUX: Launching application..."

# Check if build exists
if [ ! -d "$BUILD_DIR" ]; then
  echo "FLUX: Build directory not found. Building first..."
  "$SCRIPT_DIR/build.sh" "$BUILD_TYPE"
fi

# Find the binary
APP_PATH="${BUILD_DIR}/FLUX_artefacts/${BUILD_TYPE}/FLUX"

if [ ! -f "$APP_PATH" ]; then
  echo "FLUX: Binary not found. Building first..."
  "$SCRIPT_DIR/build.sh" "$BUILD_TYPE"
fi

echo "FLUX: Starting FLUX..."
"$APP_PATH" "${@:2}"

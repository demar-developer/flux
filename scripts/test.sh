#!/bin/bash
# FLUX — Test script

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

BUILD_DIR="${PROJECT_ROOT}/build"

echo "FLUX: Running tests..."

# Check if build exists
if [ ! -d "$BUILD_DIR" ]; then
  echo "FLUX: Build directory not found. Building first..."
  "$SCRIPT_DIR/build.sh"
fi

# Run CTest
ctest --test-dir "$BUILD_DIR" --output-on-failure "$@"

echo ""
echo "FLUX: All tests passed!"

#!/bin/bash
# FLUX — Build script

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

BUILD_TYPE="${1:-Release}"
BUILD_DIR="${PROJECT_ROOT}/build"

echo "FLUX: Building in ${BUILD_TYPE} mode..."

# Configure with ccache if available
if command -v ccache &> /dev/null; then
  echo "FLUX: Using ccache for faster rebuilds"
  cmake -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
else
  cmake -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# Build
cmake --build "$BUILD_DIR" --parallel

echo ""
echo "FLUX: Build complete!"
echo "  Binary: ${BUILD_DIR}/FLUX_artefacts/${BUILD_TYPE}/FLUX"

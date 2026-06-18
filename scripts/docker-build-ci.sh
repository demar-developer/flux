#!/bin/bash
# FLUX — Build in CI using Docker

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

cd "$PROJECT_ROOT"

# Build the image if it doesn't exist locally
if ! docker image inspect flux-builder:latest &>/dev/null; then
  echo "FLUX: Builder image not found, building..."
  docker build -t flux-builder:latest -f Dockerfile .
fi

# Run the build
echo "FLUX: Building FLUX in Docker..."
docker run --rm \
  -v "$(pwd):/workspace" \
  -v "flux-ccache:/ccache" \
  -e CCACHE_DIR=/ccache \
  -e CCACHE_MAXSIZE=2G \
  flux-builder:latest \
  bash -c "
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release &&
    cmake --build build --parallel
  "

echo ""
echo "FLUX: Build complete!"

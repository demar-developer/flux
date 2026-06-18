#!/bin/bash
# FLUX — Build and push the Docker builder image

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

IMAGE_NAME="${1:-flux-builder}"
TAG="${2:-latest}"
REGISTRY="${3:-ghcr.io/yourusername}"

echo "FLUX: Building Docker image ${IMAGE_NAME}:${TAG}..."

cd "$PROJECT_ROOT"

# Build the image
docker build -t "${IMAGE_NAME}:${TAG}" -f Dockerfile .

echo ""
echo "FLUX: Docker image built successfully!"
echo ""
echo "To use locally:"
echo "  docker run -it -v \$(pwd):/workspace ${IMAGE_NAME}:${TAG} bash"
echo ""
echo "To push to registry (optional):"
echo "  docker tag ${IMAGE_NAME}:${TAG} ${REGISTRY}/${IMAGE_NAME}:${TAG}"
echo "  docker push ${REGISTRY}/${IMAGE_NAME}:${TAG}"
echo ""
echo "To use with docker-compose:"
echo "  docker-compose run --rm builder"
echo "  docker-compose run --rm test"
echo "  docker-compose run --rm shell"

#!/bin/bash
# FLUX — Linux build dependencies setup script

set -e

echo "FLUX: Installing build dependencies..."

# Detect Ubuntu version for webkit package
UBUNTU_CODENAME=$(lsb_release -cs 2>/dev/null || echo "unknown")

echo "Detected Ubuntu codename: $UBUNTU_CODENAME"

# Base packages
PACKAGES="
  build-essential
  cmake
  ninja-build
  pkg-config
  libasound2-dev
  libjack-jackd2-dev
  libcurl4-openssl-dev
  libfreetype-dev
  libfontconfig1-dev
  libx11-dev
  libxcomposite-dev
  libxcursor-dev
  libxext-dev
  libxinerama-dev
  libxrandr-dev
  libxrender-dev
  libglu1-mesa-dev
  mesa-common-dev
  libgtk-3-dev
  ladspa-sdk
  xvfb
  git
  ccache
"

# WebKit package varies by Ubuntu version
if [ "$UBUNTU_CODENAME" = "jammy" ] || [ "$UBUNTU_CODENAME" = "focal" ]; then
  PACKAGES="$PACKAGES libwebkit2gtk-4.0-dev"
else
  # 24.04 and newer use 4.1
  PACKAGES="$PACKAGES libwebkit2gtk-4.1-dev"
fi

sudo apt-get update
sudo apt-get install -y $PACKAGES

echo ""
echo "FLUX: Build dependencies installed successfully!"
echo ""
echo "Next steps:"
echo "  ./scripts/build.sh    # Build the application"
echo "  ./scripts/test.sh     # Run tests"
echo "  ./scripts/run.sh      # Run FLUX"

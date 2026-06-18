# FLUX Builder Image
# Pre-builds JUCE 8.0.13 and Tracktion Engine v3.2.0 to speed up CI and dev builds

FROM ubuntu:24.04

# Prevent interactive prompts during apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    git \
    ccache \
    libasound2-dev \
    libjack-jackd2-dev \
    libcurl4-openssl-dev \
    libfreetype-dev \
    libfontconfig1-dev \
    libx11-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxext-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxrender-dev \
    libglu1-mesa-dev \
    mesa-common-dev \
    libgtk-3-dev \
    libwebkit2gtk-4.1-dev \
    ladspa-sdk \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

# Set up ccache
ENV CCACHE_DIR=/ccache
ENV CCACHE_MAXSIZE=2G
ENV CCACHE_COMPRESS=1
ENV CMAKE_C_COMPILER_LAUNCHER=ccache
ENV CMAKE_CXX_COMPILER_LAUNCHER=ccache
RUN mkdir -p /ccache

# Working directory for pre-building deps
WORKDIR /deps

# Pre-fetch and build JUCE 8.0.13 (commit 7c9d378)
RUN git clone --depth 1 --branch 8.0.13 https://github.com/juce-framework/JUCE.git juce && \
    cd juce && \
    git checkout 7c9d378

# Pre-fetch Tracktion Engine v3.2.0 (commit 0a5f4e6)
RUN git clone --depth 1 https://github.com/Tracktion/tracktion_engine.git && \
    cd tracktion_engine && \
    git checkout 0a5f4e6a5f53d09c89b414a44386a12df7fa1ec6

# Create a dummy CMake project to pre-build the dependencies
COPY docker/prebuild.cmake /deps/CMakeLists.txt

# Pre-build dependencies (this populates ccache)
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --parallel 4 || true

# Keep the source around for faster FetchContent
VOLUME ["/deps/juce", "/deps/tracktion_engine", "/ccache"]

# Working directory for FLUX builds
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
